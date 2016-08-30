#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QVector>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>

class GameBoard : public QWidget {
    Q_OBJECT
public:
    explicit GameBoard(QWidget *parent = 0);
    bool eventFilter(QObject *obj, QEvent *ev);
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *mouseEv);

    enum class State {
        NONE = 0,
        WHITE = 1,
        BLACK = 2
    };

    struct Chess {
        int row;
        int col;
        QColor color;
        Chess() { }
        Chess(int row, int col, QColor color): row(row), col(col), color(color) { }

        int getX() {
            return GameBoard::spacing * (col);
        }

        int getY() {
            return GameBoard::spacing * (row);
        }

        QString toString() {
            return QString("%1 %2 ").
                   arg(QString::number(row),
                       QString::number(col));
        }
    };

    QColor playerColor;
    QColor enemyColor;
    bool inRound;

    const std::vector<Chess> &getPlayerChesses() const;
    const std::vector<Chess> &getEnemyChesses() const;

private:
    bool isRunning;
    State states[15][15] = {{(State)0}};
    static int sideLength;
    static int gameSize;
    static int margin;
    static int spacing;
    bool isWin(int row, int col);
    int getdanger(int row, int col);
    std::vector<Chess> playerChesses;
    std::vector<Chess> enemyChesses;
    QVector<int> dan_row;
    QVector<int> dan_col;
    int all;
    inline int toIndex(int pixel) const;
    inline int toPoint(int index) const;
    void clear();
signals:
    void addChess(QString chessInfo);
    void win();

public slots:
    void enemyChessAdded(int row, int col);
    void onStart();
    void onTimeout();
    void onUndo();
    void onLoad(int id);
    void isdanger();

};

#endif // GAMEBOARD_H
