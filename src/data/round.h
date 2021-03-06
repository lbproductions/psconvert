#ifndef ROUND_H
#define ROUND_H

#include <QObject>

#include <QPersistenceRelations.h>
#include <QDateTime>
#include <QDate>
#define COMMA ,

#include "game.h"

class LiveDrink;
class Player;
class Point;
class Schmeisserei;

class Round : public QObject
{
    Q_OBJECT
    Q_ENUMS(WinnerParty)
    Q_ENUMS(SoloType)
    Q_ENUMS(State)
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime)
    Q_PROPERTY(QTime length READ length WRITE setLength)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    Q_PROPERTY(int soloType READ _soloType WRITE _setSoloType)
    Q_PROPERTY(bool isPflicht READ isPflicht WRITE setIsPflicht)
    Q_PROPERTY(int winnerParty READ _winnerParty WRITE _setWinnerParty)
    Q_PROPERTY(int state READ _state WRITE _setState)

    Q_PROPERTY(QMap<int COMMA int> points READ _points WRITE setPoints)

    Q_PROPERTY(QSharedPointer<Game> game READ game WRITE setGame)
    Q_PROPERTY(QList<QSharedPointer<LiveDrink> > drinks READ drinks WRITE setDrinks)
    Q_PROPERTY(QList<QSharedPointer<Schmeisserei> > schmeissereien READ schmeissereien WRITE setSchmeissereien)

    Q_PROPERTY(QSharedPointer<Player> hochzeitPlayer READ hochzeitPlayer WRITE setHochzeitPlayer)
    Q_PROPERTY(QSharedPointer<Player> trumpfabgabePlayer READ trumpfabgabePlayer WRITE setTrumpfabgabePlayer)
    Q_PROPERTY(QSharedPointer<Player> soloPlayer READ soloPlayer WRITE setSoloPlayer)
    Q_PROPERTY(QSharedPointer<Player> schweinereiPlayer READ schweinereiPlayer WRITE setSchweinereiPlayer)
    Q_PROPERTY(QSharedPointer<Player> re1Player READ re1Player WRITE setRe1Player)
    Q_PROPERTY(QSharedPointer<Player> re2Player READ re2Player WRITE setRe2Player)
    Q_PROPERTY(QSharedPointer<Player> contra1Player READ contra1Player WRITE setContra1Player)
    Q_PROPERTY(QSharedPointer<Player> contra2Player READ contra2Player WRITE setContra2Player)
    Q_PROPERTY(QSharedPointer<Player> contra3Player READ contra3Player WRITE setContra3Player)

    Q_PROPERTY(QList<QSharedPointer<Point> > pointInstances READ pointInstances WRITE setPointInstances)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:game",
                "reverserelation=rounds")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:drinks",
                "reverserelation=round")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:pointInstances",
                "reverserelation=round")

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:schmeissereien",
                "reverserelation=round")

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:hochzeitPlayer",
                "reverserelation=hochzeitRounds")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:trumpfabgabePlayer",
                "reverserelation=trumpfabgabeRounds")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:soloPlayer",
                "reverserelation=soloRounds")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:schweinereiPlayer",
                "reverserelation=schweinereiRounds")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:re1Player",
                "reverserelation=re1Rounds")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:re2Player",
                "reverserelation=re2Rounds")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:contra1Player",
                "reverserelation=contra1Rounds")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:contra2Player",
                "reverserelation=contra2Rounds")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:contra3Player",
                "reverserelation=contra3Rounds")

public:
    enum WinnerParty {
        UnknownWinnerParty,
        Re,
        Contra,
        Draw
    };

    enum State {
        UnkownState,
        Running,
        Paused,
        Finished
    };

    enum SoloType {
        UnkownSoloType,
        NoSolo,
        Fleischlos,
        BubenSolo,
        DamenSolo,
        TrumpfSolo,
        StilleHochzeit,
        SitzenGelasseneHochzeit,
        FalschGespielt,
        FarbSolo
    };

    explicit Round(QObject *parent = 0);
    ~Round();

    int number() const;
    void setNumber(int number);

    QDateTime startTime() const;

    QTime length() const;
    void setLength(const QTime &length);

    State state() const;
    void setState(const State &arg);

    QString comment() const;
    void setComment(const QString &comment);

    int points() const;
    int points(QSharedPointer<Player> player) const;
    int totalPoints(QSharedPointer<Player> player) const;
    QList<QSharedPointer<Player> > playersSortedByPlacement() const;
    int placement(QSharedPointer<Player> player) const;

    QSharedPointer<Game> game() const;

    QList<QSharedPointer<LiveDrink> > drinks() const;

    QSharedPointer<Player> cardMixer() const;
    QList<QSharedPointer<Player> > playingPlayers() const;

    SoloType soloType() const;
    void setSoloType(const SoloType &soloType);

    bool isSolo() const;

    bool isPflicht() const;
    void setIsPflicht(bool isPflicht);

    WinnerParty winnerParty() const;
    void setWinnerParty(const WinnerParty &winnerParty);

    QSharedPointer<Player> hochzeitPlayer() const;
    void setHochzeitPlayer(const QSharedPointer<Player> &hochzeitPlayer);

    QSharedPointer<Player> trumpfabgabePlayer() const;
    void setTrumpfabgabePlayer(const QSharedPointer<Player> &trumpfabgabePlayer);

    QSharedPointer<Player> soloPlayer() const;
    void setSoloPlayer(const QSharedPointer<Player> &soloPlayer);

    QSharedPointer<Player> schweinereiPlayer() const;
    void setSchweinereiPlayer(const QSharedPointer<Player> &schweinereiPlayer);

    QSharedPointer<Player> re1Player() const;
    void setRe1Player(const QSharedPointer<Player> &re1Player);

    QSharedPointer<Player> re2Player() const;
    void setRe2Player(const QSharedPointer<Player> &re2Player);

    bool isRe(QSharedPointer<Player> player) const;

    QList<QSharedPointer<Player> > rePlayers() const;
    QList<QSharedPointer<Player> > contraPlayers() const;
    QList<QSharedPointer<Player> > winners() const;
    QList<QSharedPointer<Player> > losers() const;

    QList<QSharedPointer<Schmeisserei> > schmeissereien() const;

    QSharedPointer<Player> contra1Player() const;
    void setContra1Player(QSharedPointer<Player> arg);

    QSharedPointer<Player> contra2Player() const;
    void setContra2Player(QSharedPointer<Player> arg);

    QSharedPointer<Player> contra3Player() const;
    void setContra3Player(QSharedPointer<Player> arg);

private:
    void setDrinks(const QList<QSharedPointer<LiveDrink> > &drinks);
    void setGame(const QSharedPointer<Game> &game);
    void setStartTime(const QDateTime &startTime);
    QList<QSharedPointer<Point> > pointInstances() const;
    void setPointInstances(const QList<QSharedPointer<Point> > &pointInstances);
    void setSchmeissereien(const QList<QSharedPointer<Schmeisserei> > &schmeissereien);

    QMap<int, int> _points() const;
    void setPoints(const QMap<int, int> &_points);

    int cardMixerPosition() const;

    int _state() const;
    void _setState(int state);

    int _soloType() const;
    void _setSoloType(int type);

    int _winnerParty() const;
    void _setWinnerParty(int party);

    int m_number;
    QDateTime m_startTime;
    State m_state;
    QTime m_length;
    QString m_comment;
    SoloType m_soloType;
    bool m_soloIsPflicht;
    mutable WinnerParty m_winnerParty;
    mutable QMap<int, int> m_points;

    QpWeakRelation<Game> m_game;
    QpStrongRelation<LiveDrink> m_drinks;
    QpStrongRelation<Point> m_pointInstances;
    QpStrongRelation<Schmeisserei> m_schmeissereien;
    QpWeakRelation<Player> m_hochzeitPlayer;
    QpWeakRelation<Player> m_trumpfabgabePlayer;
    QpWeakRelation<Player> m_soloPlayer;
    QpWeakRelation<Player> m_schweinereiPlayer;
    QpWeakRelation<Player> m_re1Player;
    QpWeakRelation<Player> m_re2Player;
    mutable QpWeakRelation<Player> m_contra1Player;
    mutable QpWeakRelation<Player> m_contra2Player;
    mutable QpWeakRelation<Player> m_contra3Player;
};

#undef COMMA



#endif // ROUND_H
