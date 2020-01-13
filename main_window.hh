/*
####################################################################
# TIE-02201 Ohjelmointi 2: Perusteet, K2019                        #
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Project4: Snake: The Revengeance                                 #
# Program description: "While revenge is served cold and vengeance #
#                       is served hot revengeance is often seen    #
#                       served with Cajun with a side of onion     #
#                       rings and often dumplings in the Orient."  #
#                       - urbandictionary                          #
#                                                                  #
# File: main_window.hh                                             #
# Description: Declares a class implementing a UI for the game.    #
#                                                                  #
# Author: Niko Pärssinen, 283244, niko.parssinen@tuni.fi           #
####################################################################
*/

#ifndef PRG2_SNAKE2_MAINWINDOW_HH
#define PRG2_SNAKE2_MAINWINDOW_HH

#include "ui_main_window.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QTimer>
#include <random>

// Struct for a point's coordinates.
struct Point {
    int x;
    int y;
}; // struct Point

/* \class MainWindow
 * \brief Implements the main window through which the game is played.
 */
class MainWindow: public QMainWindow {
    Q_OBJECT

public:

    /* \brief Construct a MainWindow.
     *
     * \param[in] parent The parent widget of this MainWindow.
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /* \brief Destruct a MainWindow.
     */
    ~MainWindow() override = default;

    /* \brief Change the Snake's bearing when certain keys get pressed.
     *
     * \param[in] event Contains data on pressed and released keys.
     */
    void keyPressEvent(QKeyEvent* event) override;


private slots:

    /* \brief Start the game.
     */
    void on_playButton_clicked();

    /* \brief Move the Snake by a square and check for collisions.
     *
     * The game ends if a wall or the Snake itself gets in the way.
     * When a food gets eaten a point is gained and the Snake grows.
     */
    void moveSnake();

    /* \brief Updates the snake on the display.
     */
    void updateSnake();

    /* \brief Move the food when its eaten.
     */
    void moveFood();

    /* \brief Generates a random number in the wanted range.
     */
    int rng();

    /* \brief If the game is won.
     */
    void gameWon();

    /* \brief If the game is lost.
     */
    void gameLost();

    /* \brief Checks if the space is occupied by the snakes body.
     */
    bool spaceOccupied(Point tile);

    /* \brief Resets the game and restarts it again.
     */
    void on_restartButton_clicked();

private:

    /* \brief Make the play field visible and fit it into the view.
     *
     * Should be called every time the field's size changes.
     */
    void adjustSceneArea();

    Ui::MainWindow ui_;                         /**< Accesses the UI widgets. */
    QGraphicsRectItem* snake_tile = nullptr;    /**< The snake tile in the scene. */
    QGraphicsRectItem* food_ = nullptr;         /**< The food item in the scene. */
    Point food_point;                           /**< Food's coordinate struct. */
    QGraphicsScene scene_;                      /**< Manages drawable objects. */
    QTimer timer_;                              /**< Triggers the Snake to move. */
    std::default_random_engine rng_;            /**< Randomizes food locations. */
    std::string direction_;                     /**< Snake's direction. */
    int side_length = 20;                       /**< Area's side length. */
    int time_used;                              /**< Time used in ticks of the timer. */
    int minutes_;                               /**< Full minutes passed since starting. */
    std::vector<Point> snake_;                  /**< Structure for the snake itself. */
    bool lose_ = false;                         /**< Truth value about if the game is lost. */



};  // class MainWindow


#endif  // PRG2_SNAKE2_MAINWINDOW_HH
