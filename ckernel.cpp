#include "ckernel.h"

once_flag CKernel::m_once;
unique_ptr<CKernel> CKernel::self;

#define FUNMAP(a) m_ProToFun[a - PRO_BASE]

CKernel::CKernel(): m_sock_accept(new Tcpsock(IP, PORT)), m_epoll(new Myepoll), m_sql(new CMysql)
{
    cout << __func__ << endl;
    // 设置函数映射
    SetFun();

    // 连接数据库
    if (m_sql->ConnectMysql("localhost", "root", "hedong666", "WeChat")) {
        cout << "sql Connect success!" << endl;
    }
    else {
        cout << "sql Connect failed!" << endl;
        return;
    }

    m_sock_accept->Listen();

    int sk = m_sock_accept->GetSock();

    m_epoll->AddNode(sk);

    vector<epoll_event>* p = nullptr;

    while (1) {
        int num;
        p = m_epoll->Wait(num);
        if (!p) return;
        for (int i = 0; i < num; ++i) {
            epoll_event x = p->at(i);
            if (x.data.fd == sk) {

                Tcpsock chat(m_sock_accept->Accept());
                m_epoll->AddNode(chat.GetSock());
            }
            else {
                char* buf;
                Tcpsock* t = new Tcpsock(x.data.fd);
                if (!(buf = t->Read())) {
                    cout << "客户端已退出" << endl;
                    t->Close();
                    m_epoll->DelNode(t->GetSock());
                    continue;
                }

                // 将buf的数据还原
                CJson* json = new CJson(buf);

                // 回收buf
                delete[] buf, buf = nullptr;

                int type = json->json_get_int("type");
                if (type < PRO_BASE || type >= PRO_BASE + PRO_CNT) {
                    cout << "type error" << endl;
                    return;
                }

                FUNMAP(type)(json, t);
            }
        }
    }
}

void CKernel::SetFun()
{
    cout << __func__ << endl;
    memset(m_ProToFun, 0, sizeof(m_ProToFun));

    FUNMAP(LOG_RQ) = bind(&CKernel::DealLogRq, this, placeholders::_1, placeholders::_2);
    FUNMAP(REG_RQ) = bind(&CKernel::DealRegRq, this, placeholders::_1, placeholders::_2);
}

void Send(Tcpsock* sock, CJson& rs)
{
    const char* con = rs.json_to_string();
    sock->Write(con, strlen(con) + 1);
    delete sock;
}

void CKernel::DealLogRq(CJson* buf, Tcpsock* sock)
{
    cout << __func__ << endl;

    static char query[200];
    static list<string> res;

    // 处理信息
    string tel = buf->json_get_string("phone");
    string pass = buf->json_get_string("pass");

    // 回收空间
    delete buf, buf = nullptr;

    CJson rs;
    rs.json_add_value("type", LOG_RS);

    // 判断用户是否存在
//    char query[100];
    sprintf(query, "select pass, id, icon, feeling, name from t_user where tel = '%s'", tel.c_str());
//    list<string> res;
    if (!m_sql->SelectMysql(query, 5, res)) {
        cout << "查询失败:" << query << endl;
        delete sock;
        return;
    }

    // 用户不存在
    if (res.empty()) {
        rs.json_add_value("result", USER_MISS);
        Send(sock, rs);
        return;
    }

    // 密码错误
    if (pass != res.front()) {
        rs.json_add_value("result", PASS_ERROR);
        Send(sock, rs);
        return;
    }
    res.pop_front();

    // 登录成功
    int id = stoi(res.front()); res.pop_front();
    int icon = stoi(res.front()); res.pop_front();
    string feeling = res.front(); res.pop_front();
    string name = res.front(); res.pop_front();
    m_MapIdToInfo.AddValue(id, UserInfo(name, sock->GetSock()));
    rs.json_add_value("result", LOG_SUCCESS);
    rs.json_add_value("id", id);
    rs.json_add_value("icon", icon);
    rs.json_add_value("feeling", feeling.c_str());
    rs.json_add_value("name", name.c_str());
//    const char* con = rs.json_to_string();
    Send(sock, rs);
//    sock->Write(con, strlen(con) + 1);
//    delete sock;
}

void CKernel::DealRegRq(CJson* buf, Tcpsock* sock)
{
    cout << __func__ << endl;

    static char query[200];
    static list<string> res;

    // 获得信息
    string pass = buf->json_get_string("pass");
    string tel = buf->json_get_string("phone");
    string name = buf->json_get_string("name");

    // 提取完json信息，回收json
    delete buf; buf = nullptr;

//    cout << name << ' ' << tel << ' ' << pass << endl;

    CJson rs;
    rs.json_add_value("type", REG_RS);

    // 名字是否存在
    sprintf(query, "select * from t_user where name = '%s'", name.c_str());
//    cout << query << endl;
    m_sql->SelectMysql(query, 6, res);
    if (res.size()) {
        rs.json_add_value("result", NAME_USED);
        Send(sock, rs);
//        // 回收空间
//        delete buf, buf = nullptr;
        return;
    }

    // 电话是否使用
    sprintf(query, "select * from t_user where tel = '%s'", tel.c_str());
    m_sql->SelectMysql(query, 6, res);
    if (res.size()) {
        rs.json_add_value("result", TEL_USED);
        Send(sock, rs);
//        // 回收空间
//        delete buf, buf = nullptr;
        return;
    }

    // 注册成功
    // 写入数据库
    cout << name << ' ' << tel << ' ' << pass << endl;
    sprintf(query, "insert into t_user (tel, pass, name) values('%s', '%s', '%s')", tel.c_str(), pass.c_str(), name.c_str());
    if (!m_sql->UpdataMysql(query)) {
        cout << "写入数据库失败！:" << query <<  endl;
        return;
    }
    cout << query << endl;
    rs.json_add_value("result", REG_SUCCESS);
    Send(sock, rs);
}

CKernel *CKernel::GetKernel()
{
    call_once(m_once, []() {
       self.reset(new CKernel);
    });
    return self.get();
}
