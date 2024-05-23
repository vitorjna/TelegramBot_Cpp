#include "FunUtils.h"

QString FunUtils::getQuoteFuturama()
{
    static QVector<QString> szaQuotes;

    if (szaQuotes.isEmpty()) {

        QString szFile(QStringLiteral("FuturamaQuotes.txt"));

        QFile myFile(szFile);

        if (myFile.open(QFile::ReadOnly) == true) {
            szaQuotes.reserve(300);

            QTextStream myTextStream(&myFile);

            while (myTextStream.atEnd() == false) {
                szaQuotes.append(myTextStream.readLine());
            }
        }
    }

    if (szaQuotes.size() > 0) {
        int nRandom = rand() % szaQuotes.size();
        return szaQuotes.at(nRandom);

    } else {
        return QStringLiteral("Kill all humans, must kill all humans!");
    }
}
