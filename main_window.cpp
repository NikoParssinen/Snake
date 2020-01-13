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
# File: main_window.cpp                                            #
# Description: Defines a class implementing a UI for the game.     #
#                                                                  #
# Author: Niko Pärssinen, 283244, niko.parssinen@tuni.fi           #
####################################################################
*/

#include "main_window.hh"
#include <QGraphicsRectItem>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent)
{
    ui_.setupUi(this);
    ui_.graphicsView->setScene(&scene_);
    ui_.restartButton->setDisabled(true);

    connect(&timer_, &QTimer::timeout, this, &MainWindow::moveSnake);
}

// Changes direction depending on what key is pressed but will not change to the opposite direction.
void MainWindow::keyPressEvent(QKeyEvent* event)
{

    if (event->key() == Qt::Key_W && direction_ != "down") {
        direction_ = "up";
    }
    else if (event->key() == Qt::Key_A && direction_ != "right") {
        direction_ = "left";
    }
    else if (event->key() == Qt::Key_S && direction_ != "up") {
        direction_ = "down";
    }
    else if (event->key() == Qt::Key_D && direction_ != "left") {
        direction_ = "right";
    }
}

// Pressing play button the game will start.
void MainWindow::on_playButton_clicked()
{
    // Create snake
    const QRectF rect(0, 0, 1, 1);
    const QPen pen(Qt::white, 0);
    const QBrush brush(Qt::black);
    snake_tile = scene_.addRect(rect, pen, brush);
    snake_tile->setPos(side_length/2, side_length/2);
    Point point = {side_length/2, side_length/2};
    snake_.push_back(point);

    // Create area
    adjustSceneArea();

    direction_ = "down";

    // Create first food
    const QBrush brush2(Qt::red);
    food_ = scene_.addRect(rect, pen, brush2);
    int x = rng();
    int y = rng();
    food_->setPos(x, y);
    food_point = {x, y};

    // Start timer
    time_used = 0;
    minutes_ = 0;
    timer_.start(200);

    ui_.playButton->setDisabled(true);
}

// Moves the snake to the wanted direction
void MainWindow::moveSnake()
{
    // Add time
    ++time_used;
    if (time_used/5 >= 60) {
        ++minutes_;
        time_used = 0;
    }

    // Head's new location
    Point new_head = {snake_.back().x, snake_.back().y};

    if (direction_ == "up") {
        new_head = {snake_.back().x, snake_.back().y - 1};
    }
    else if (direction_ == "left") {
        new_head = {snake_.back().x - 1, snake_.back().y};
    }
    else if (direction_ == "down") {
        new_head = {snake_.back().x, snake_.back().y + 1};
    }
    else if (direction_ == "right") {
        new_head = {snake_.back().x + 1, snake_.back().y};
    }

    if (snake_.size() > 1) {
        const Point& neck = snake_[snake_.size() - 2];

        if (new_head.x == neck.x && new_head.y == neck.y) {

            // Changing the direction back to what it was, because the snake can't move to its neck.
            if (direction_ == "up") {
                direction_ = "down";
                new_head = {snake_.back().x, snake_.back().y + 1};
            }
            else if (direction_ == "left") {
                direction_ = "right";
                new_head = {snake_.back().x + 1, snake_.back().y};
            }
            else if (direction_ == "down") {
                direction_ = "up";
                new_head = {snake_.back().x, snake_.back().y - 1};
            }
            else if (direction_ == "right") {
                direction_ = "left";
                new_head = {snake_.back().x - 1, snake_.back().y};
            }

        }

    }

    // Snake hit one of the vertical walls
    if (new_head.x < 0 || new_head.x > side_length - 1) {
        gameLost();
    }

    // Snake hit one of the horizontal walls
    if (new_head.y < 0 || new_head.y > side_length - 1) {
        gameLost();
    }

    // Snake hits its own body
    if (spaceOccupied(new_head)) {
        gameLost();
    }

    // Move the snake.
    snake_.push_back(new_head);
    updateSnake();

    // If the snake's head is going to eat the food then move it
    // and not remove the tail meaning that the snake gets bigger.
    if (new_head.x == food_point.x && new_head.y == food_point.y) {
        int size = snake_.size();

        // Check if game is won
        if(size > side_length * side_length) {
            gameWon();
        }

        moveFood();
    } else {
        snake_.erase(snake_.begin());
    }


}

// This updates snake on the window.
void MainWindow::updateSnake()
{
    const QRectF snake_rect(0, 0, 1, 1);
    const QPen pen(Qt::white, 0);
    const QBrush brush(Qt::gray); // Snake's body is gray
    const QBrush brush2(Qt::black); // Snake's head is black

    // Update head
    snake_tile = scene_.addRect(snake_rect, pen, brush2);
    snake_tile->setPos(snake_.back().x, snake_.back().y);

    // Update body
    int i = 1;
    int size = snake_.size();
    while(i < size - 1)
    {
        snake_tile = scene_.addRect(snake_rect, pen, brush);
        snake_tile->setPos(snake_.at(i).x, snake_.at(i).y);
        ++i;
    }

    // Return the rectangle to white when the tail moves out of that tile
    const QBrush brush3(Qt::white);
    snake_tile = scene_.addRect(snake_rect, pen, brush3);
    snake_tile->setPos(snake_.begin()->x, snake_.begin()->y);

}

// Creates the food in a new location.
void MainWindow::moveFood()
{
    const QRectF rect(0, 0, 1, 1);
    const QPen pen(Qt::white, 0);
    const QBrush brush2(Qt::red);
    food_ = scene_.addRect(rect, pen, brush2);
    while(true) {
        int x = rng();
        int y = rng();
        food_point = {x, y};

        // Snakeless square found, stop moving the food around.
        if (!spaceOccupied(food_point)) {
            food_->setPos(x, y);
            return;
        }
    }
}

// Generates a random number in the wanted range
int MainWindow::rng()
{
    std::uniform_int_distribution<int> distribution(0, side_length-1);
    int random_number = distribution(rng_);
    return random_number;
}

// If the game is won
void MainWindow::gameWon()
{
    ui_.graphicsView->setDisabled(true);
    snake_ = {};
    timer_.stop();
    QString s = QString::number(time_used/5);
    QString m = QString::number(minutes_);
    ui_.textBrowser->setText("You won! Great job!\n\nTime used: "+ m + " minutes and " + s + " seconds");
    ui_.restartButton->setDisabled(false);
}

// If the game is lost
void MainWindow::gameLost()
{
    ui_.graphicsView->setDisabled(true);
    snake_ = {};
    timer_.stop();
    QString s = QString::number(time_used/5);
    QString m = QString::number(minutes_);
    ui_.textBrowser->setText("You lost!\n\nTime used: " + m + " minutes and " + s + " seconds");
    ui_.restartButton->setDisabled(false);
}

// Checks if the space is occupied by the snakes body
bool MainWindow::spaceOccupied(Point tile)
{
    // If space is occupied, return true
    int i = 1;
    int size = snake_.size();
    while(i < size) {
            if(snake_.at(i).x == tile.x && snake_.at(i).y == tile.y) {
                return true;
            }
            ++i;
    }
    return false;
}

// Creates the 21x21 area.
void MainWindow::adjustSceneArea() {
    const QRectF area(0, 0, side_length, side_length);
    scene_.setSceneRect(area);
    ui_.graphicsView->fitInView(area);
}

// Resets the game and restarts it again.
void MainWindow::on_restartButton_clicked()
{
    // Reset everything to white on the graphics view
    const QRectF rect(0, 0, 1, 1);
    const QPen pen(Qt::white, 0);
    const QBrush brush(Qt::white);
    int x = 0;
    int y = 0;
    while(y <= side_length) {
        while(x <= side_length) {
            QGraphicsRectItem* tile  = scene_.addRect(rect, pen, brush);
            tile->setPos(x, y);
            ++x;
        }
        ++y;
        x = 0;
    }

    ui_.graphicsView->setDisabled(false);
    ui_.restartButton->setDisabled(true);

    // Restart the game
    on_playButton_clicked();
}
