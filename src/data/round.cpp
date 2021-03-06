#include "round.h"

#include "game.h"
#include "livedrink.h"
#include "player.h"
#include "point.h"
#include "schmeisserei.h"

#include <limits>



Round::Round(QObject *parent) :
    QObject(parent),
    m_number(-1),
    m_state(Round::UnkownState),
    m_soloType(UnkownSoloType),
    m_soloIsPflicht(false),
    m_winnerParty(UnknownWinnerParty),
    m_game("game", this),
    m_drinks("drinks",this),
    m_pointInstances("pointInstances",this),
    m_schmeissereien("schmeissereien",this),
    m_hochzeitPlayer("hochzeitPlayer",this),
    m_trumpfabgabePlayer("trumpfabgabePlayer",this),
    m_soloPlayer("soloPlayer",this),
    m_schweinereiPlayer("schweinereiPlayer",this),
    m_re1Player("re1Player",this),
    m_re2Player("re2Player",this),
    m_contra1Player("contra1Player",this),
    m_contra2Player("contra2Player",this),
    m_contra3Player("contra3Player",this)
{
}

Round::~Round()
{
}

QList<QSharedPointer<LiveDrink> > Round::drinks() const
{
    return m_drinks.resolveList();
}

void Round::setDrinks(const QList<QSharedPointer<LiveDrink> > &drinks)
{
    m_drinks.clear();
    m_drinks.relate(drinks);
}

QSharedPointer<Game> Round::game() const
{
    return m_game.resolve();
}

void Round::setGame(const QSharedPointer<Game> &game)
{
    m_game.relate(game);
}

QMap<int, int> Round::_points() const
{
    if(m_points.isEmpty()) {
        foreach(QSharedPointer<Point> p, pointInstances()) {
            m_points.insert(Qp::primaryKey(p->player()), p->points());
        }
    }

    return m_points;
}

void Round::setPoints(const QMap<int, int> &points)
{
    m_points = points;
}

QString Round::comment() const
{
    return m_comment;
}

void Round::setComment(const QString &comment)
{
    m_comment = comment;
}

int Round::points() const
{
    QSharedPointer<Player> player = re1Player();

    if(!player)
        return 0;

    int p = points(player);
    if(isSolo())
        p /= 3;
    return p;
}

int Round::points(QSharedPointer<Player> player) const
{
    return _points().value(Qp::primaryKey(player));
}

int Round::totalPoints(QSharedPointer<Player> player) const
{
    int ps = points(player);

    int previousRoundNumber = number() - 1;
    QList<QSharedPointer<Round> > rounds = game()->rounds();
    if(previousRoundNumber < 0 || previousRoundNumber >= rounds.size())
        return ps;

    ps += rounds.at(previousRoundNumber)->totalPoints(player);

    return ps;
}

QList<QSharedPointer<Player> > Round::playersSortedByPlacement() const
{
    QList<QSharedPointer<Player> > ps = game()->players();

    QMultiMap<int, QSharedPointer<Player>> helperMap;
    foreach(QSharedPointer<Player> player, ps) {
        helperMap.insert(totalPoints(player), player);
    }

    return Qp::reversed(helperMap.values());
}

int Round::placement(QSharedPointer<Player> player) const
{
    int place = 0;
    QList<QSharedPointer<Player> > ps = playersSortedByPlacement();
    int points = std::numeric_limits<int>::max();
    foreach(QSharedPointer<Player> p, ps) {
        int pPoints = totalPoints(p);
        if(pPoints < points) {
            ++place;
            points = pPoints;
        }

        if(p == player)
            return place;
    }

    Q_ASSERT(false);
    return 0;
}

QSharedPointer<Player> Round::cardMixer() const
{
    int cardMixerPos = cardMixerPosition();
    if(cardMixerPos < 0)
        return QSharedPointer<Player>();

    QList<QSharedPointer<Player> > ps = game()->players();
    if(ps.size() <= cardMixerPos)
        return QSharedPointer<Player>();

    return ps.at(cardMixerPos);
}

QList<QSharedPointer<Player> > Round::playingPlayers() const
{
    QList<QSharedPointer<Player> > ps = game()->players();
    if(ps.isEmpty())
        return QList<QSharedPointer<Player> >();

    QList<QSharedPointer<Player> > result;

    int countPlayers = ps.size();
    int cardMixerPos = cardMixerPosition();
    for(int i = cardMixerPos + 1, anzahl = 0; anzahl < 4; ++i, ++anzahl)
    {
        i = i % countPlayers;

        if(countPlayers > 5 && anzahl+1 == countPlayers / 2)
            ++i;

        i = i % countPlayers;

        QSharedPointer<Player> player = ps.at(i);
        result.append(player);
    }

    Q_ASSERT(result.size() == 4);
    return result;
}

int Round::cardMixerPosition() const
{
    int countPlayers = game()->players().size();
    if(countPlayers == 0)
        return -1;

    return number() % countPlayers;
}

int Round::_state() const
{
    return static_cast<int>(state());
}

void Round::_setState(int state)
{
    setState(static_cast<Round::State>(state));
}

int Round::_soloType() const
{
    return static_cast<int>(soloType());
}

void Round::_setSoloType(int type)
{
    setSoloType(static_cast<Round::SoloType>(type));
}

int Round::_winnerParty() const
{
    return static_cast<int>(winnerParty());
}

void Round::_setWinnerParty(int party)
{
    setWinnerParty(static_cast<Round::WinnerParty>(party));
}


QTime Round::length() const
{
    return m_length;
}

void Round::setLength(const QTime &length)
{
    m_length = length;
}

QDateTime Round::startTime() const
{
    return m_startTime;
}

void Round::setStartTime(const QDateTime &startTime)
{
    m_startTime = startTime;
}

int Round::number() const
{
    return m_number;
}

void Round::setNumber(int number)
{
    m_number = number;
}

QList<QSharedPointer<Point> > Round::pointInstances() const
{
    return m_pointInstances.resolveList();
}

void Round::setPointInstances(const QList<QSharedPointer<Point> > &pointInstances)
{
    m_pointInstances.clear();
    m_pointInstances.relate(pointInstances);
}

Round::WinnerParty Round::winnerParty() const
{
    if(m_winnerParty == UnknownWinnerParty
            && !_points().isEmpty()) {
        int p = points();
        if(p < 0)
            m_winnerParty = Contra;
        else if(p == 0)
            m_winnerParty = Draw;
        else
            m_winnerParty = Re;
    }

    return m_winnerParty;
}

void Round::setWinnerParty(const WinnerParty &winnerParty)
{
    m_winnerParty = winnerParty;
}

bool Round::isPflicht() const
{
    return m_soloIsPflicht;
}

void Round::setIsPflicht(bool soloIsPflicht)
{
    m_soloIsPflicht = soloIsPflicht;
}

Round::SoloType Round::soloType() const
{
    return m_soloType;
}

void Round::setSoloType(const SoloType &soloType)
{
    m_soloType = soloType;
}

bool Round::isSolo() const
{
    return soloType() != NoSolo && soloType() != UnkownSoloType;
}

QSharedPointer<Player> Round::re2Player() const
{
    return m_re2Player.resolve();
}

void Round::setRe2Player(const QSharedPointer<Player> &re2Player)
{
    m_re2Player.relate(re2Player);
}

QSharedPointer<Player> Round::re1Player() const
{
    return m_re1Player.resolve();
}

void Round::setRe1Player(const QSharedPointer<Player> &re1Player)
{
    m_re1Player.relate(re1Player);
}

QSharedPointer<Player> Round::schweinereiPlayer() const
{
    return m_schweinereiPlayer.resolve();
}

void Round::setSchweinereiPlayer(const QSharedPointer<Player> &schweinereiPlayer)
{
    m_schweinereiPlayer.relate(schweinereiPlayer);
}

QSharedPointer<Player> Round::soloPlayer() const
{
    return m_soloPlayer.resolve();
}

void Round::setSoloPlayer(const QSharedPointer<Player> &soloPlayer)
{
    m_soloPlayer.relate(soloPlayer);
}

QSharedPointer<Player> Round::trumpfabgabePlayer() const
{
    return m_trumpfabgabePlayer.resolve();
}

void Round::setTrumpfabgabePlayer(const QSharedPointer<Player> &trumpfabgabePlayer)
{
    m_trumpfabgabePlayer.relate(trumpfabgabePlayer);
}

QSharedPointer<Player> Round::hochzeitPlayer() const
{
    return m_hochzeitPlayer.resolve();
}

void Round::setHochzeitPlayer(const QSharedPointer<Player> &hochzeitPlayer)
{
    m_hochzeitPlayer.relate(hochzeitPlayer);
}

QList<QSharedPointer<Schmeisserei> > Round::schmeissereien() const
{
    return m_schmeissereien.resolveList();
}

void Round::setSchmeissereien(const QList<QSharedPointer<Schmeisserei> > &schmeissereien)
{
    m_schmeissereien.relate(schmeissereien);
}


QList<QSharedPointer<Player> > Round::winners() const
{
    WinnerParty winner = winnerParty();
    if(winner == Re)
        return rePlayers();
    else if(winner == Contra)
        return contraPlayers();

    return QList<QSharedPointer<Player> >();
}

QList<QSharedPointer<Player> > Round::losers() const
{
    WinnerParty winner = winnerParty();
    if(winner == Re)
        return contraPlayers();
    else if(winner == Contra)
        return rePlayers();

    return QList<QSharedPointer<Player> >();
}


QList<QSharedPointer<Player> > Round::rePlayers() const
{
    QList<QSharedPointer<Player> > result;
    result.append(re1Player());

    if(!isSolo()) {
        result.append(re2Player());
    }

    Q_ASSERT(result.size() == 1 || result.size() == 2);
    return result;
}

QList<QSharedPointer<Player> > Round::contraPlayers() const
{    
    QList<QSharedPointer<Player> > result;
    QSharedPointer<Player> re1 = re1Player();
    if(!re1)
        return QList<QSharedPointer<Player> >();

    QSharedPointer<Player> re2 = re2Player();

    foreach(QSharedPointer<Player> p, playingPlayers()) {
        if(re1.data() != p.data() && re2.data() != p.data())
            result.append(p);
    }

    Q_ASSERT(result.size() == 2 || result.size() == 3);
    return result;
}

Round::State Round::state() const
{
    return m_state;
}

void Round::setState(const State &arg)
{
    m_state = arg;
}

QSharedPointer<Player> Round::contra1Player() const
{
    QSharedPointer<Player> result = m_contra1Player.resolve();
    if(result)
        return result;

    QList<QSharedPointer<Player> > players = contraPlayers();
    if(players.isEmpty())
        return QSharedPointer<Player>();

    m_contra1Player.relate(players.first());
    return players.first();
}

QSharedPointer<Player> Round::contra2Player() const
{
    QSharedPointer<Player> result = m_contra2Player.resolve();
    if(result)
        return result;

    QList<QSharedPointer<Player> > players = contraPlayers();
    if(players.size() < 2)
        return QSharedPointer<Player>();

    m_contra2Player.relate(players.at(1));
    return players.at(1);
}

QSharedPointer<Player> Round::contra3Player() const
{
    QSharedPointer<Player> result = m_contra3Player.resolve();
    if(result)
        return result;

    QList<QSharedPointer<Player> > players = contraPlayers();
    if(players.size() < 3)
        return QSharedPointer<Player>();

    m_contra3Player.relate(players.at(2));
    return players.at(2);
}

void Round::setContra1Player(QSharedPointer<Player> arg)
{
    m_contra1Player.relate(arg);
}

void Round::setContra2Player(QSharedPointer<Player> arg)
{
    m_contra2Player.relate(arg);
}

void Round::setContra3Player(QSharedPointer<Player> arg)
{
    m_contra3Player.relate(arg);
}
