/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class Character;
class Watson;
class Sherlock;

class TestStudyInPink;

enum ItemType
{
    MAGIC_BOOK,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};

class MapElement
{
    friend class TestStudyInPink;

protected:
    ElementType type;

public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement
{
    friend class TestStudyInPink;

public:
    Path();
};
class Wall : public MapElement
{
    friend class TestStudyInPink;

public:
    Wall();
};

class FakeWall : public MapElement
{
    friend class TestStudyInPink;

private:
    int req_exp;

public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Position
{
    friend class TestStudyInPink;
private:
    int r, c;

public:
    static const Position npos;

    Position(int r = 0, int c = 0);

    Position(const string &str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
};

class MovingObject
{
    friend class TestStudyInPink;

protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    string getName();
    int getIndex();
    virtual void move() = 0;
    virtual string str() const = 0;
};

class Map
{
    friend class TestStudyInPink;

private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

class Character : public MovingObject
{
    friend class TestStudyInPink;

protected:
    string moving_rule;
    int moving_ruleIndex;
    int hp, exp;

public:
    Character(int index, const Position pos, Map *map, const string &name);
    int getHp() const;
    int getExp() const;
    void roundUp();
    void setHp(int hp);
    void setExp(int exp);
    void setPos(Position temp);
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
};

class Sherlock : public Character
{
    friend class TestStudyInPink;
private:
    BaseBag *sBag;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    ~Sherlock();
    Position getNextPosition() override;
    void move();
    string str() const override;
    BaseBag *getBag();

    void counterRobotS(RobotS *robotS);
    void counterRobotW(RobotW *robotW);
    void counterRobotSW(RobotSW *robotSW);
    bool counterRobotC(RobotC *robotC);
    void counterWatson(Watson *watson);
    // ...
};

class Watson : public Character
{
    friend class TestStudyInPink;
private:
    BaseBag *wBag;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    ~Watson();
    Position getNextPosition() override;
    void move();
    string str() const override;
    BaseBag *getBag();

    void counterRobotS(RobotS *robotS);
    void counterRobotW(RobotW *robotW);
    void counterRobotSW(RobotSW *robotSW);
    void counterRobotC(RobotC *robotC);
    void counterSherlock(Sherlock *sherlock);
};

class Criminal : public Character
{
    friend class TestStudyInPink;

private:
    // TODO
    Sherlock *sherlock;
    Watson *watson;
    Position previous;
    int count;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    ~Criminal();
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    Position getPreviousPosition();
    int getCount() const;
    // ...
};

class ArrayMovingObject
{
    friend class TestStudyInPink;

private:
    // TODO
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();
    int getSize() const;
    bool isFull() const;
    bool counterEvent(int index);
    bool add(MovingObject *mv_obj);
    MovingObject *get(int i);
    string str() const;
};

class Configuration
{
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    // TODO
    int map_num_rows, map_num_cols, max_num_moving_objects, num_walls, num_fake_walls, num_steps, sherlock_init_hp, sherlock_init_exp, watson_init_hp, watson_init_exp;
    Position *arr_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule, watson_moving_rule;
    Position sherlock_init_pos, watson_init_pos, criminal_init_pos;

public:
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag,...

class Robot : public MovingObject
{
    friend class TestStudyInPink;

protected:
    Criminal *criminal;
    RobotType robot_type;
    BaseItem *item;

public:
    Robot(int index, Position pos, Map *map, RobotType robot_type, Criminal *criminal, const string &name = "");
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual int getDistance() const = 0;
    int getCardinalNumber(int p);
    BaseItem *getItem();
    static Robot *create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
};

class RobotC : public Robot
{
    friend class TestStudyInPink;

public:
    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal);
    ~RobotC();
    Position getNextPosition() override;
    void move() override;
    int getDistance(Character *character) const;
    int getDistance() const override{
        return 1;
    };
    string str() const override;
};

class RobotS : public Robot
{
    friend class TestStudyInPink;

protected:
    Sherlock *sherlock;

public:
    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *Sherlock);
    ~RobotS();
    Position getNextPosition();
    void move() override;
    int getDistance() const override;
    string str() const override;
};

class RobotW : public Robot
{
    friend class TestStudyInPink;

protected:
    Watson *watson;

public:
    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson);
    ~RobotW();
    Position getNextPosition();
    void move() override;
    int getDistance() const override;
    string str() const override;
};

class RobotSW : public Robot
{
    friend class TestStudyInPink;

protected:
    Sherlock *sherlock;
    Watson *watson;

public:
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    ~RobotSW();
    Position getNextPosition();
    void move() override;
    int getDistance() const override;
    string str() const override;
};

class BaseItem
{
    friend class TestStudyInPink;
protected:
    ItemType type;

public:
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
    virtual ItemType getType() const = 0;
    virtual string str() const = 0;
};

class MagicBook : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
};

class EnergyDrink : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
};

class FirstAid : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
};

class ExcemptionCard : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
};

class PassingCard : public BaseItem
{
    friend class TestStudyInPink;

protected:
    string challenge;

public:
    PassingCard(string challenge);
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
    string getChallenge();
};

class Node
{
    friend class TestStudyInPink;

public:
    BaseItem *item;
    Node *next;
    Node(BaseItem *item);
};

class BaseBag
{
    friend class TestStudyInPink;

protected:
    Node *head;
    Character *character;
    int max;
    int count;

public:
    BaseBag(Character *character);
    ~BaseBag();
    void remove();
    bool findItem(ItemType item);
    virtual bool insert(BaseItem *item);
    virtual BaseItem *get();
    virtual BaseItem *get(ItemType itemType);
    virtual string str() const;
    int getCount() const;
};

class SherlockBag : public BaseBag
{
    friend class TestStudyInPink;

public:
    SherlockBag(Sherlock *sherlock);
};

class WatsonBag : public BaseBag
{
    friend class TestStudyInPink;

public:
    WatsonBag(Watson *watson);
};

class StudyPinkProgram
{
    friend class TestStudyInPink;
private:
    // Sample attributes
    Configuration *config;

    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

    Map *map;
    ArrayMovingObject *arr_mv_objs;
    bool stop;

public:
    StudyPinkProgram(const string &config_file_path);

    bool isStop() const;

    void printResult() const;

    void printStep(int si) const;

    void run(bool verbose);

    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
