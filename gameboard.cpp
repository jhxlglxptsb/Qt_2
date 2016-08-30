#include "gameboard.h"
#include "saveloaddialog.h"
#include "infodialog.h"
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMessageBox>
#include <QSettings>

namespace {

GameBoard::State toState(QColor color) {
    return (color == Qt::white)? GameBoard::State::WHITE: GameBoard::State::BLACK;
}

}

int GameBoard::sideLength = 420;
int GameBoard::gameSize = 15;
int GameBoard::margin = 20;
int GameBoard::spacing = GameBoard::sideLength / (GameBoard::gameSize - 1);

GameBoard::GameBoard(QWidget *parent) :
    QWidget(parent),
    inRound(false),
    isRunning(false) {
    // TODO
    playerColor = Qt::white;
    enemyColor = Qt::black;
    installEventFilter(this);
    all = 0;
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            states[i][j] = State::NONE;
}

bool GameBoard::eventFilter(QObject *obj, QEvent *ev) {
    if (obj != this) {
        return false;
    }

    if (ev->type() == QEvent::MouseButtonDblClick) {
        return true;
    }

    if (ev->type() != QEvent::MouseButtonPress) {
        return false;
    }

    if (!inRound)
        return true;

    QMouseEvent *mouseEv = static_cast<QMouseEvent *>(ev);

    if (states[toIndex(mouseEv->y())][toIndex(mouseEv->x())] != State::NONE) {
        return true;
    }

    return false;
}

void GameBoard::paintEvent(QPaintEvent *ev) {

    QPainter p(this);
    p.setBrush(Qt::gray);
    p.setPen(Qt::NoPen);
    p.drawRect(0, 0, sideLength + 2 * margin, sideLength + 2 * margin);
    p.translate(margin, margin);
    p.setPen(QPen(Qt::blue, 1));

    for (int i = 0; i < gameSize; i++) {
        p.drawLine(0, i * spacing, sideLength, i * spacing);
        p.drawLine(i * spacing, 0, i * spacing, sideLength);
    }

    p.setPen(QPen(Qt::black, 5));
    p.drawPoint(3 * spacing, 3 * spacing);
    p.drawPoint(3 * spacing, 11 * spacing);
    p.drawPoint(11 * spacing, 3 * spacing);
    p.drawPoint(11 * spacing, 11 * spacing);
    p.drawPoint(7 * spacing, 7 * spacing);

    p.setPen(Qt::NoPen);
    p.setBrush(playerColor);

    for (int i = 0; i < playerChesses.size(); i++) {
        if (playerChesses[i].row == -1)
            continue;

        p.drawEllipse(QPoint(playerChesses[i].getX(), playerChesses[i].getY()), (int)(spacing * 0.4), (int)(spacing * 0.4));
    }

    p.setBrush(enemyColor);

    for (int i = 0; i < enemyChesses.size(); i++) {
        if (enemyChesses[i].row == -1)
            continue;

        p.drawEllipse(QPoint(enemyChesses[i].getX(), enemyChesses[i].getY()), (int)(spacing * 0.4), (int)(spacing * 0.4));
    }
    if(all >= 2)
    {
        for(int i =0; i<dan_row.size(); i++)
        {
            // qDebug()<<dan_row[i]<<" "<<dan_col[i];
            QPixmap pix;
            QString filename = ":Desk\zhadan.jpg";
            qDebug()<<pix.load(filename);
            p.drawPixmap(toPoint(dan_row[i]), toPoint(dan_col[i]), spacing, spacing ,pix);
        }
        all = 0;
    }
}

void GameBoard::mousePressEvent(QMouseEvent *mouseEv) {
    int row = toIndex(mouseEv->y());
    int col = toIndex(mouseEv->x());

    states[row][col] = toState(playerColor);
    Chess chess(row, col, playerColor);
    playerChesses.push_back(chess);
    emit addChess(chess.toString());
    update();
    inRound = false;

    if (isWin(row, col)) {
        emit win();
    }

}

bool GameBoard::isWin(int row, int col) {
    State state = states[row][col];
    int cnt = 1;

    for (int i = row - 1; i >= 0; i--) {
        if (states[i][col] != state)
            break;

        ++cnt;
    }

    for (int i = row + 1; i < gameSize; i++) {
        if (states[i][col] != state)
            break;

        ++cnt;
    }

    if (cnt >= 5)
        return true;

    cnt = 1;

    for (int i = col - 1; i >= 0; i--) {
        if (states[row][i] != state)
            break;

        ++cnt;
    }

    for (int i = col + 1; i < gameSize; i++) {
        if (states[row][i] != state)
            break;

        ++cnt;
    }

    if (cnt >= 5)
        return true;

    cnt = 1;

    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (states[i][j] != state)
            break;

        ++cnt;
    }

    for (int i = row + 1, j = col + 1; i < gameSize && j < gameSize; i++, j++) {
        if (states[i][j] != state)
            break;

        ++cnt;
    }

    if (cnt >= 5)
        return true;

    cnt = 1;

    for (int i = row - 1, j = col + 1; i >= 0 && j < gameSize; i--, j++) {
        if (states[i][j] != state)
            break;

        ++cnt;
    }

    for (int i = row + 1, j = col - 1; i < gameSize && j >= 0; i++, j--) {
        if (states[i][j] != state)
            break;

        ++cnt;
    }

    if (cnt >= 5)
        return true;

    return false;
}

void GameBoard::isdanger()
{
    dan_col.clear();
    dan_row.clear();
    all = 0;
    for(int i =0; i < 15; i++)
        for(int j = 0; j < 15; j++)
        {
            int jud = getdanger(i, j);
            all = all + jud;
            if(jud)
            {
                dan_row.push_back(i);
                dan_col.push_back(j);
            }
        }
    qDebug()<<all;
    update();
}

int GameBoard::getdanger(int row, int col)
{
    int con = 0;
    State state = toState(enemyColor);
    int cnt = 1;
    int side = 0;
    for (int i = row - 1; i >= 0; i--) {
        if (states[i][col] != state)
        {
            if(states[i][col] == State::NONE)
                side++;
            break;
        }

        ++cnt;
    }

    for (int i = row + 1; i < gameSize; i++) {
        if (states[i][col] != state)
        {
            if(states[i][col] == State::NONE)
                side++;
            break;
        }

        ++cnt;
    }

    if ((cnt == 3&&side == 2)||(cnt == 4&&side == 1))
       con++;

    cnt = 1;
    side = 0;
    for (int i = col - 1; i >= 0; i--) {
        if (states[row][i] != state)
        {
            if(states[row][i] == State::NONE)
                side++;
            break;
        }

        ++cnt;
    }

    for (int i = col + 1; i < gameSize; i++) {
        if (states[row][i] != state)
        {
            if(states[row][i] == State::NONE)
                side++;
            break;
        }
        ++cnt;
    }

    if ((cnt == 3&&side == 2)||(cnt == 4&&side == 1))
       con++;

    cnt = 1;
    side = 0;

    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (states[i][j] != state)
        {
            if(states[i][j] == State::NONE)
                side++;
            break;
        }

        ++cnt;
    }

    for (int i = row + 1, j = col + 1; i < gameSize && j < gameSize; i++, j++) {
        if (states[i][j] != state)
        {
            if(states[i][j] == State::NONE)
                side++;
            break;
        }

        ++cnt;
    }

    if ((cnt == 3&&side == 2)||(cnt == 4&&side == 1))
       con++;

    cnt = 1;
    side = 0;
    for (int i = row - 1, j = col + 1; i >= 0 && j < gameSize; i--, j++) {
        if (states[i][j] != state)
        {
            if(states[i][j] == State::NONE)
                side++;
            break;
        }
        ++cnt;
    }

    for (int i = row + 1, j = col - 1; i < gameSize && j >= 0; i++, j--) {
        if (states[i][j] != state)
        {
            if(states[i][j] == State::NONE)
                side++;
            break;
        }

        ++cnt;
    }

    if ((cnt == 3&&side == 2)||(cnt == 4&&side == 1))
       con++;

    return con;
}

const std::vector<GameBoard::Chess> &GameBoard::getEnemyChesses() const {
    return enemyChesses;
}

const std::vector<GameBoard::Chess> &GameBoard::getPlayerChesses() const {
    return playerChesses;
}

inline int GameBoard::toIndex(int pixel) const {
    return ((pixel - margin + 0.5 * spacing) / spacing);
}

inline int GameBoard::toPoint(int index) const {
    return (index * spacing);
}

void GameBoard::clear() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            states[i][j] = State::NONE;
        }
    }

    playerChesses.clear();
    enemyChesses.clear();
}

void GameBoard::enemyChessAdded(int row, int col) {
    enemyChesses.push_back(Chess(row, col, enemyColor));

    if (row != -1 && col != -1)
        states[row][col] = toState(enemyColor);

    update();
    inRound = true;
}

void GameBoard::onStart() {
    //    isRunning = true;
}

void GameBoard::onTimeout() {
    Chess chess(-1, -1, playerColor);
    playerChesses.push_back(chess);
    emit addChess(chess.toString());
}

void GameBoard::onUndo() {
    auto chess1 = playerChesses.back();

    if (chess1.row != -1 && chess1.col != -1)
        states[chess1.row][chess1.col] = State::NONE;

    playerChesses.pop_back();

    auto chess2 = enemyChesses.back();

    if (chess2.row != -1 && chess2.col != -1)
        states[chess2.row][chess2.col] = State::NONE;

    enemyChesses.pop_back();

    update();
}

void GameBoard::onLoad(int id) {
    clear();

    QSettings settings;

    int size = settings.beginReadArray(QString("white") + QString::number(id));

    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        Chess chess;
        chess.row = settings.value("row").toInt();
        chess.col = settings.value("col").toInt();
        states[chess.row][chess.col] = State::WHITE;
        chess.color = Qt::white;
        ((playerColor == Qt::white)? playerChesses: enemyChesses).push_back(chess);
    }

    settings.endArray();

    size = settings.beginReadArray(QString("black") + QString::number(id));
    qDebug() << "black" << size;

    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        Chess chess;
        chess.row = settings.value("row").toInt();
        chess.col = settings.value("col").toInt();

        if (chess.row != -1 && chess.col != -1)
            states[chess.row][chess.col] = State::BLACK;

        chess.color = Qt::black;
        ((playerColor == Qt::white)? enemyChesses: playerChesses).push_back(chess);
    }

    settings.endArray();

    update();
}
