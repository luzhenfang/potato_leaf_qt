#ifndef RESULT_H
#define RESULT_H

#include <QObject>
#include <QString>




//namespace ResultCode{
//    static int OK=200;
//}


class Result : public QObject
{
    Q_OBJECT
public:
    explicit Result(QObject *parent = nullptr);
    Result(const Result& ref);
    explicit Result(QObject*parent,int resCode,QString resMsg,QString resType,QString resConf)
        :QObject(parent),
          code{resCode},
          msg{resMsg},
          type{resType},
          conf{resConf}{

    }

    Result(const Result&& ref){
        this->code=ref.code;
        this->conf=ref.conf;
        this->msg=ref.msg;
        this->type=ref.type;
    }



    int getCode() const;
    void setCode(int newCode);

    const QString &getMsg() const;
    void setMsg(const QString &newMsg);

    const QString &getType() const;
    void setType(const QString &newType);

    const QString &getConf() const;
    void setConf(const QString &newConf);

   void operator=(const Result& ref);

private:
    int code;
    QString msg;
    QString type;
    QString conf;
signals:

};

#endif // RESULT_H
