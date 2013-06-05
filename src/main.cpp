#include <QApplication>

#include <data/drink.h>
#include <data/game.h>
#include <data/livedrink.h>
#include <data/place.h>
#include <data/player.h>
#include <data/round.h>
#include <data/point.h>
#include <data/schmeisserei.h>

#include <QSqlDatabase>
#include <QStyle>
#include <QPersistence.h>
#include <QPersistenceDatabaseSchema.h>
#include "../lib/QPersistence/src/sqlquery.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(a.arguments().size() != 2) {
        qDebug() << "Usage: psconvert <sqlite_database>";
        return 0;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(a.arguments().at(1));
    db.open();

    Qp::registerMappableTypes<int, int>();
    Qp::setDatabase(db);
    Qp::registerClass<LiveDrink>();
    Qp::registerClass<Place>();
    Qp::registerClass<Player>();
    Qp::registerClass<Drink>();
    Qp::registerClass<Game>();
    Qp::registerClass<Round>();
    Qp::registerClass<Point>();
    Qp::registerClass<Schmeisserei>();

    QpDatabaseSchema schema(db);

    schema.renameTable("drinks", "drink");
    schema.renameTable("games", "game");
    schema.renameTable("livegamedrinks", "livedrink");
    schema.renameTable("places", "place");
    schema.renameTable("players", "player");
    schema.renameTable("points", "point");
    schema.renameTable("positions", "_Qp_REL_game_players__player_games");
    schema.renameTable("playerplaceassignments", "_Qp_REL_place_players__player_places");
    schema.renameTable("rounds", "round");
    schema.renameTable("schmeissereis", "schmeisserei");

    schema.dropTable("categorieassignments");
    schema.dropTable("categories");
    schema.dropTable("samplerows");
    schema.dropTable("rules");

    schema.renameColumn("drink", "id", "_Qp_ID");
    schema.renameColumn("drink", "type", "typeString");
    schema.renameColumn("drink", "size", "volume");
    schema.renameColumn("drink", "alc", "alcoholByVolume");
    schema.addColumn("drink", "picture", "BLOB");
    schema.addColumn("drink", "type", "INTEGER");

    schema.renameColumn("game", "id", "_Qp_ID");
    schema.renameColumn("game", "type", "typeString");
    schema.renameColumn("game", "date", "creationTime");
    schema.renameColumn("game", "siteId", "_Qp_FK_site");
    schema.addColumn("game", "type", "INTEGER");
    schema.addColumn("game", "mitPflichtsolo", "BOOLEAN");

    schema.renameColumn("livedrink", "id", "_Qp_ID");
    schema.renameColumn("livedrink", "roundId", "_Qp_FK_round");
    schema.renameColumn("livedrink", "playerId", "_Qp_FK_player");
    schema.renameColumn("livedrink", "drinkId", "_Qp_FK_drink");

    schema.renameColumn("place", "id", "_Qp_ID");
    schema.renameColumn("place", "plz", "postalCode");
    schema.renameColumn("place", "ort", "city");
    schema.renameColumn("place", "strasse", "street");
    schema.renameColumn("place", "nummer", "houseNumber");
    schema.addColumn("place", "cityEmblem", "BLOB");

    schema.renameColumn("player", "id", "_Qp_ID");
    schema.renameColumn("player", "size", "height");
    schema.renameColumn("player", "gender", "genderString");
    schema.addColumn("player", "gender", "INTEGER");


    schema.renameColumn("point", "id", "_Qp_ID");
    schema.renameColumn("point", "roundId", "_Qp_FK_round");
    schema.renameColumn("point", "playerId", "_Qp_FK_player");

    schema.renameColumn("_Qp_REL_game_players__player_games", "id", "_Qp_ID");
    schema.renameColumn("_Qp_REL_game_players__player_games", "gameId", "_Qp_FK_game");
    schema.renameColumn("_Qp_REL_game_players__player_games", "playerId", "_Qp_FK_player");

    schema.renameColumn("_Qp_REL_place_players__player_places", "id", "_Qp_ID");
    schema.renameColumn("_Qp_REL_place_players__player_places", "placeId", "_Qp_FK_place");
    schema.renameColumn("_Qp_REL_place_players__player_places", "playerId", "_Qp_FK_player");


    schema.renameColumn("round", "id", "_Qp_ID");
    schema.renameColumn("round", "gameId", "_Qp_FK_game");
    schema.renameColumn("round", "state", "state_old");
    schema.renameColumn("round", "db_state", "state");
    schema.renameColumn("round", "doko_hochzeitPlayerId", "_Qp_FK_hochzeitPlayer");
    schema.renameColumn("round", "doko_re1PlayerId", "_Qp_FK_re1Player");
    schema.renameColumn("round", "doko_re2PlayerId", "_Qp_FK_re2Player");
    schema.renameColumn("round", "doko_schweinereiPlayerId", "_Qp_FK_schweinereiPlayer");
    schema.renameColumn("round", "doko_soloPlayerId", "_Qp_FK_soloPlayer");
    schema.renameColumn("round", "doko_soloType", "soloTypeString");
    schema.renameColumn("round", "doko_trumpfabgabePlayerId", "_Qp_FK_trumpfabgabePlayer");
    schema.renameColumn("round", "doko_winner", "winnerParty");
    schema.renameColumn("round", "length", "length_BACKUP");
    schema.addColumn("round", "length", "TEXT");
    schema.addColumn("round", "points", "TEXT");
    schema.addColumn("round", "isPflicht", "BOOLEAN");
    schema.addColumn("round", "soloType", "INTEGER");
    schema.addColumn("round", "_Qp_FK_contra1Player", "INTEGER");
    schema.addColumn("round", "_Qp_FK_contra2Player", "INTEGER");
    schema.addColumn("round", "_Qp_FK_contra3Player", "INTEGER");

    QpSqlQuery query1(db);
    query1.exec("INSERT INTO schmeisserei (_Qp_FK_player,_Qp_FK_round,type)  "
                "SELECT playerId,roundId,type FROM doko_schmeissereien");
    schema.renameColumn("schmeisserei", "id", "_Qp_ID");
    schema.renameColumn("schmeisserei", "playerId", "_Qp_FK_player");
    schema.renameColumn("schmeisserei", "roundId", "_Qp_FK_round");
    schema.renameColumn("schmeisserei", "type", "typeString");
    schema.addColumn("schmeisserei", "type", "INTEGER");

    QpSqlQuery query(db);
    query.exec("DELETE FROM game WHERE live = 'false'");
    query.exec("DELETE FROM game WHERE NOT typeString = 'Doppelkopf'");

    query.exec("DELETE FROM round WHERE NOT EXISTS "
               "(SELECT 1 FROM game WHERE game._qp_id = round._qp_fk_game)");
    query.exec("DELETE FROM _qp_rel_game_players__player_games WHERE NOT EXISTS "
               "(SELECT 1 FROM game WHERE game._qp_id = _qp_rel_game_players__player_games._qp_fk_game)");

    query.exec("DELETE FROM point WHERE NOT EXISTS "
               "(SELECT 1 FROM round WHERE round._Qp_id = point._Qp_FK_round)");
    query.exec("DELETE from schmeisserei WHERE NOT EXISTS "
               "(SELECT 1 FROM round WHERE round._qp_id = schmeisserei._qp_fk_round)");

    query.exec("UPDATE round SET isPflicht = 'false' WHERE doko_soloPflicht = 'false' OR doko_soloPflicht IS EMPTY");
    query.exec("UPDATE round SET isPflicht = 'true' WHERE doko_soloPflicht = 'true'");
    query.exec("UPDATE game SET mitPflichtsolo = 'true'");
    query.exec("UPDATE game SET mitPflichtsolo = 'false' "
               "WHERE doko_mitPflichtsolo = 'false' OR doko_mitPflichtsolo IS NULL OR doko_mitPflichtsolo = 'null'");
    query.exec("UPDATE drink SET type = 1 WHERE typeString = 'Beer'");
    query.exec("UPDATE drink SET type = 4 WHERE typeString = 'Schnapps'");
    query.exec("UPDATE drink SET type = 2 WHERE typeString = 'Mixed beer'");
    query.exec("UPDATE drink SET type = 5 WHERE typeString = 'Wine'");
    query.exec("UPDATE game SET type = 1 WHERE typeString = 'Doppelkopf'");
    query.exec("UPDATE game SET type = 2 WHERE typeString = 'Skat'");
    query.exec("UPDATE game SET type = 3 WHERE typeString = 'Hearts'");
    query.exec("UPDATE game SET type = 4 WHERE typeString = 'Prognose'");
    query.exec("UPDATE player SET gender = 1 WHERE genderString = 'male'");
    query.exec("UPDATE player SET gender = 2 WHERE genderString = 'female'");
    query.exec("UPDATE schmeisserei SET type = 1 WHERE typeString = '5 Könige'");
    query.exec("UPDATE schmeisserei SET type = 2 WHERE typeString = 'Trumpfabgabe nicht genommen'");
    query.exec("UPDATE schmeisserei SET type = 3 WHERE typeString = 'Zu wenig Trumpf' OR typeString = '< 3 Trumpf'");
    query.exec("UPDATE schmeisserei SET type = 4 WHERE typeString = '>90 Punkte'");
    query.exec("UPDATE round SET soloType = 1 WHERE soloTypeString IS EMPTY");
    query.exec("UPDATE round SET soloType = 1 WHERE soloTypeString IS NULL");
    query.exec("UPDATE round SET soloType = 1 WHERE soloTypeString = 'null'");
    query.exec("UPDATE round SET soloType = 2 WHERE soloTypeString = 'Fleischlos'");
    query.exec("UPDATE round SET soloType = 2 WHERE soloTypeString = 'Fleischlos-Solo'");
    query.exec("UPDATE round SET soloType = 3 WHERE soloTypeString = 'Buben'");
    query.exec("UPDATE round SET soloType = 3 WHERE soloTypeString = 'Buben-Solo'");
    query.exec("UPDATE round SET soloType = 4 WHERE soloTypeString = 'Damen'");
    query.exec("UPDATE round SET soloType = 4 WHERE soloTypeString = 'Damen-Solo'");
    query.exec("UPDATE round SET soloType = 5 WHERE soloTypeString = 'Trumpf'");
    query.exec("UPDATE round SET soloType = 6 WHERE soloTypeString = 'StilleHochzeit'");
    query.exec("UPDATE round SET soloType = 8 WHERE soloTypeString = 'Schwarz'");
    query.exec("UPDATE round SET soloType = 9 WHERE soloTypeString = 'Farben'");
    query.exec("UPDATE round SET length = length_BACKUP");


    QList<QSharedPointer<Player> > players = Qp::readAll<Player>();
    QList<QSharedPointer<Game> > games = Qp::readAll<Game>();
    QList<QSharedPointer<Round> > rounds = Qp::readAll<Round>();
    QList<QSharedPointer<Point> > points = Qp::readAll<Point>();

    foreach(QSharedPointer<Round> p, rounds) {
        Qp::update(p);
    }

    schema.dropTable("dokoofflinegamebuddys");
    schema.dropTable("doko_livegameinformations");
    schema.dropTable("doko_offlinegamebuddys");
    schema.dropTable("doko_offlinegameinformations");
    schema.dropTable("doko_roundinformations");
    schema.dropTable("doko_schmeissereien");
    schema.dropTable("leagueplayerassignments");
    schema.dropTable("leagues");
    schema.dropTable("offlinegameinformations");
    schema.dropTable("leagues");
    schema.dropTable("skat_livegameinformations");
    schema.dropTable("skat_roundinformations");

    schema.dropColumns("drink", QStringList() << "typeString");
    schema.dropColumns("game", QStringList() << "typeString" << "live" << "length"
                       << "skat_gameEnd" << "skat_maxPoints" << "skat_maxRounds" << "skat_mitRamschen"
                       << "offline_length" << "doko_soloPflicht" << "doko_mitPflichtsolo"
                       << "doko_mitHochzeit" << "doko_mitSchmeisserei"<< "doko_mitSchweinerei"<< "doko_mitSolo"
                       << "doko_mitTrumpfabgabe"<< "doko_mitBubensolo"<< "doko_mitDamensolo"<< "doko_mitFarbsolo"
                       << "doko_mitFleischloss"<< "doko_mitFuenfKoenige"<< "doko_mitNeunzigPunkte"<< "doko_mitTrumpfabgabeSchmeisserei"
                       << "doko_mitTrumpfsolo"<< "doko_mitFleischlos"<< "doko_mitSchwarzsolo" << "doko_mitZuWenigTrumpf");
    schema.dropColumns("place", QStringList() << "playerId");
    schema.dropColumns("player", QStringList() << "avatarPath" << "genderString");
    schema.dropColumns("round", QStringList() << "state_old" << "doko_mitspielerId" << "doko_soloPflicht" << "length_BACKUP"
                       << "skat_gameType" << "skat_solistPlayerId" << "skat_trumpfColor" << "soloTypeString");
    schema.dropColumns("schmeisserei", QStringList() << "typeString");

    schema.dropTable("point");

    return 0;
}
