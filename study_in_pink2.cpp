#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

//<========================================================= CLASS MAPELEMENT =================================================================>

MapElement::MapElement(ElementType in_type)
{
    this->type = in_type;
};

MapElement::~MapElement(){};

ElementType MapElement::getType() const
{
    return type;
};

//<============================================================================================================================================>

//<==================================================== CLASS PATH, WALL, FAKEWALL ===========================================================>

Path::Path() : MapElement(PATH){};

Wall::Wall() : MapElement(WALL){};

FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL)
{
    this->req_exp = in_req_exp;
};

int FakeWall::getReqExp() const
{
    return req_exp;
};

//<============================================================================================================================================>

//<============================================================= CLASS MAP ====================================================================>

Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
{
    this->num_rows = num_rows;
    this->num_cols = num_cols;

    if(num_rows == 0 || num_cols == 0) return;

    map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; i++)
    {
        map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; j++)
        {
            map[i][j] = new Path();
        }
    }

    for (int fake_wall = 0; fake_wall < num_fake_walls; fake_wall++)
    {
        int in_req_exp = (array_fake_walls[fake_wall].getRow() * 257 + array_fake_walls[fake_wall].getCol() * 139 + 89) % 900 + 1;
        delete map[array_fake_walls[fake_wall].getRow()][array_fake_walls[fake_wall].getCol()];
        map[array_fake_walls[fake_wall].getRow()][array_fake_walls[fake_wall].getCol()] = new FakeWall(in_req_exp);
    }

    for (int wall = 0; wall < num_walls; wall++)
    {
        delete map[array_walls[wall].getRow()][array_walls[wall].getCol()];
        map[array_walls[wall].getRow()][array_walls[wall].getCol()] = new Wall();
    }
}

Map::~Map()
{
    if(num_rows == 0 || num_cols == 0) return;
    
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            delete map[i][j];
        }
    }
    
    for (int i = 0; i < num_rows; i++)
    {
        delete[] map[i];
    }
    
    delete map;
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    int posCol = pos.getCol(), posRow = pos.getRow();
    if (posCol < 0 || posCol >= num_cols || posRow < 0 || posRow >= num_rows)
    {
        return false;
    }
    else if (map[posRow][posCol]->getType() == WALL)
    {
        return false;
    }
    else if (map[posRow][posCol]->getType() == PATH)
    {
        return true;
    }
    else if (map[posRow][posCol]->getType() == FAKE_WALL)
    {
        if (mv_obj->getName() == "Watson")
        {
            Watson *watson = dynamic_cast<Watson *>(mv_obj);
            FakeWall *fakewall = dynamic_cast<FakeWall *>(map[posRow][posCol]);
            int reqexp = fakewall->getReqExp();
            if (watson->getExp() > reqexp)
            {
                return true;
            }
            return false;
        }
        else if (mv_obj->getName() == "Sherlock")
        {
            return true;
        }
        else if (mv_obj->getName() == "Criminal")
        {
            return true;
        }
        else if (mv_obj->getName().find("Robot") != string::npos)
        {
            return true;
        }
    }
    return true;
}
//<============================================================================================================================================>

//<=============================================================== CLASS POSITION =============================================================>

const Position Position::npos = Position(-1, -1);

Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
};

Position::Position(const string &str_pos)
{
    string row, col;
    int i = 1;
    while (str_pos[i] != ',')
    {
        row += str_pos[i];
        i++;
    }
    i++;
    while (str_pos[i] != ')')
    {
        col += str_pos[i];
        i++;
    }
    this->r = stoi(row);
    this->c = stoi(col);
};

int Position::getRow() const
{
    return r;
}

int Position::getCol() const
{
    return c;
}

void Position::setRow(int r)
{
    this->r = r;
};

void Position::setCol(int c)
{
    this->c = c;
}

string Position::str() const
{
    return "(" + to_string(r) + "," + to_string(c) + ")";
}

bool Position::isEqual(int in_r, int in_c) const
{
    if (in_r == r && in_c == c)
    {
        return true;
    }
    return false;
}

//<============================================================================================================================================>

//<========================================================= CLASS MOVING OBJECT ==============================================================>

MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
};

MovingObject::~MovingObject(){};

Position MovingObject::getCurrentPosition() const
{
    return pos;
}

string MovingObject::getName()
{
    return name;
}

int MovingObject::getIndex()
{
    return index;
};

//<============================================================================================================================================>

//<============================================================= CLASS CHARACTER ==============================================================>

Character::Character(int index, const Position pos, Map *map, const string &name) : MovingObject(index, pos, map, name)
{
    this->moving_ruleIndex = 0;
};

int Character::getHp() const
{
    return hp;
};

int Character::getExp() const
{
    return exp;
};

void Character::roundUp()
{
    if (hp > 500)
    {
        hp = 500;
    }
    else if (hp < 0)
    {
        hp = 0;
    }

    if (exp > 900)
    {
        exp = 900;
    }
    else if (exp < 0)
    {
        exp = 0;
    }
}

void Character::setExp(int exp)
{
    this->exp = exp;
};

void Character::setHp(int hp)
{
    this->hp = hp;
};

void Character::setPos(Position temp)
{
    this->pos = temp;
}

//<============================================================================================================================================>

//<============================================================== CLASS SHERLOCK ==============================================================>

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Sherlock")
{
    this->moving_rule = moving_rule;
    this->hp = init_hp;
    this->exp = init_exp;
    this->roundUp();
    this->sBag = new SherlockBag(this);
};

Sherlock::~Sherlock()
{
    delete sBag;
};

Position Sherlock::getNextPosition()
{
    if (exp == 0)
        return Position::npos;
    if (moving_ruleIndex == moving_rule.length())
    {
        moving_ruleIndex = 0;
    }
    Position next = pos;
    if (moving_rule[moving_ruleIndex] == 'L')
    {
        next.setCol(pos.getCol() - 1);
    }
    else if (moving_rule[moving_ruleIndex] == 'R')
    {
        next.setCol(pos.getCol() + 1);
    }
    else if (moving_rule[moving_ruleIndex] == 'U')
    {
        next.setRow(pos.getRow() - 1);
    }
    else if (moving_rule[moving_ruleIndex] == 'D')
    {
        next.setRow(pos.getRow() + 1);
    }
    if (map->isValid(next, this))
    {
        return next;
    }
    return Position::npos;
}

void Sherlock::move()
{
    Position next = this->getNextPosition();
    if (next.isEqual(-1, -1) == false)
    {
        pos.setCol(next.getCol());
        pos.setRow(next.getRow());
    }
    moving_ruleIndex++;
};

string Sherlock::str() const
{
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

BaseBag *Sherlock::getBag()
{
    return sBag;
}

void Sherlock::counterRobotS(RobotS *robotS)
{

    bool reduce = true;
    if (this->sBag->findItem(EXCEMPTION_CARD) == true && hp % 2 != 0)
    {
        (this->sBag->get(EXCEMPTION_CARD))->use(this, robotS);
        reduce = false;
    }

    if (this->exp > 400)
    {
        this->sBag->insert(robotS->getItem());
    }
    else
    {
        if (reduce)
        {
            this->exp = (int)(this->exp * 0.9 + 0.99999);
            this->roundUp();
        }
    }

    BaseItem *after = this->sBag->get();
    if (after != NULL)
    {
        after->use(this, NULL);
        this->roundUp();
    }
}

void Sherlock::counterRobotW(RobotW *robotW)
{
    if (this->sBag->findItem(EXCEMPTION_CARD) == true && hp % 2 != 0)
    {
        (this->sBag->get(EXCEMPTION_CARD))->use(this, robotW);
    }

    this->sBag->insert(robotW->getItem());

    BaseItem *after = this->sBag->get();
    if (after != NULL)
    {
        after->use(this, NULL);
        this->roundUp();
    }
}

void Sherlock::counterRobotSW(RobotSW *robotSW)
{
    bool reduce = true;

    if (this->sBag->findItem(EXCEMPTION_CARD) == true && hp % 2 != 0)
    {
        (this->sBag->get(EXCEMPTION_CARD))->use(this, robotSW);
        reduce = false;
    }

    if (this->hp > 335 && this->exp > 300)
    {
        this->sBag->insert(robotSW->getItem());
    }
    else
    {
        if (reduce)
        {
            this->exp = (int)(this->exp * 0.85 + 0.9999);
            this->hp = (int)(this->hp * 0.85 + 0.9999);
            this->roundUp();
        }
    }

    BaseItem *after = this->sBag->get();
    if (after != NULL)
    {
        after->use(this, NULL);
        this->roundUp();
    }
}

bool Sherlock::counterRobotC(RobotC *robotC)
{
    bool ans;

    if (this->sBag->findItem(EXCEMPTION_CARD) && hp % 2 != 0)
    {
        (this->sBag->get(EXCEMPTION_CARD))->use(this, robotC);
    }

    if (this->exp > 500)
    {
        ans = true;
    }
    else
    {
        this->sBag->insert(robotC->getItem());
    }

    BaseItem *after = this->sBag->get();
    if (after != NULL)
    {
        after->use(this, NULL);
        this->roundUp();
    }

    return ans;
}

void Sherlock::counterWatson(Watson *watson)
{
    if (this->sBag->findItem(PASSING_CARD) == true && watson->getBag()->findItem(EXCEMPTION_CARD) == true && this->sBag->getCount() < 13 && watson->getBag()->getCount() < 15)
    {
        BaseItem * passingCard;
        BaseItem * excemptionCard;
        while (this->sBag->findItem(PASSING_CARD) && watson->getBag()->getCount() < 15){
            passingCard = this->sBag->get(PASSING_CARD);
            watson->getBag()->insert(passingCard);
        }
    
        while(watson->getBag()->findItem(EXCEMPTION_CARD) && this->sBag->getCount() < 13){
            excemptionCard = watson->getBag()->get(EXCEMPTION_CARD);
            this->sBag->insert(excemptionCard);
        }
    }
}

//<============================================================================================================================================>

//<============================================================== CLASS WATSON ================================================================>

Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson")
{
    this->moving_rule = moving_rule;
    this->hp = init_hp;
    this->exp = init_exp;
    this->roundUp();
    this->wBag = new WatsonBag(this);
};

Watson::~Watson()
{
    delete wBag;
};

Position Watson::getNextPosition()
{
    if (exp == 0)
        return Position::npos;
    if (moving_ruleIndex == moving_rule.length())
    {
        moving_ruleIndex = 0;
    }
    Position next = pos;
    if (moving_rule[moving_ruleIndex] == 'L')
    {
        next.setCol(pos.getCol() - 1);
    }
    else if (moving_rule[moving_ruleIndex] == 'R')
    {
        next.setCol(pos.getCol() + 1);
    }
    else if (moving_rule[moving_ruleIndex] == 'U')
    {
        next.setRow(pos.getRow() - 1);
    }
    else if (moving_rule[moving_ruleIndex] == 'D')
    {
        next.setRow(pos.getRow() + 1);
    }
    if (map->isValid(next, this))
    {
        return next;
    }
    return Position::npos;
}

void Watson::move()
{
    Position next = this->getNextPosition();
    if (next.isEqual(-1, -1) == false)
    {
        pos.setCol(next.getCol());
        pos.setRow(next.getRow());
    }
    moving_ruleIndex++;
};

string Watson::str() const
{
    return ("Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]");
}

BaseBag *Watson::getBag()
{
    return wBag;
};

void Watson::counterRobotS(RobotS *robotS)
{
    if (this->wBag->findItem(PASSING_CARD) && hp % 2 == 0)
    {
        (this->wBag->get(PASSING_CARD))->use(this, robotS);
        this->roundUp();
    }

    BaseItem *after = this->wBag->get();

    if (after != NULL)
    {
        after->use(this, NULL);
        this->roundUp();
    }
}

void Watson::counterRobotW(RobotW *robotW)
{
    bool valid = false;
    if (this->wBag->findItem(PASSING_CARD) && hp % 2 == 0)
    {
        (this->wBag->get(PASSING_CARD))->use(this, robotW);
        this->roundUp();
        valid = true;
    }
    if (this->hp > 350 || valid)
    {
        this->wBag->insert(robotW->getItem());
    }
    else
    {
        this->hp = (int)(this->hp * 0.95 + 0.9999);
        this->roundUp();
    }

    BaseItem *after = this->wBag->get();
    if (after != NULL)
    {
        after->use(this, NULL);
        this->roundUp();
    }
}

void Watson::counterRobotSW(RobotSW *robotSW)
{
    bool valid = false;
    if (this->wBag->findItem(PASSING_CARD) && hp % 2 == 0)
    {
        (this->wBag->get(PASSING_CARD))->use(this, robotSW);
        this->roundUp();
        valid = true;
    }
    if ((this->exp > 600 && this->hp > 165) || valid)
    {
        this->wBag->insert(robotSW->getItem());
    }
    else
    {
        this->hp = (int)(this->hp * 0.85 + 0.9999);
        this->exp = (int)(this->exp * 0.85 + 0.9999);
        this->roundUp();
    }

    BaseItem *after = this->wBag->get();
    if (after != NULL)
    {
        after->use(this, NULL);
        this->roundUp();
    }
}

void Watson::counterRobotC(RobotC *robotC)
{
    bool valid;
    int temp_hp = hp;
    if (this->wBag->findItem(PASSING_CARD) && hp % 2 == 0)
    {
        (this->wBag->get(PASSING_CARD))->use(this, robotC);
        this->roundUp();
    }

    this->wBag->insert(robotC->getItem());

    BaseItem *after = this->wBag->get();
    if (after != NULL)
    {
        after->use(this, NULL);
        this->roundUp();
    }
}

void Watson::counterSherlock(Sherlock *sherlock)
{
    if (sherlock->getBag()->findItem(PASSING_CARD) == true && this->wBag->findItem(EXCEMPTION_CARD) == true && this->wBag->getCount() < 15 && sherlock->getBag()->getCount() < 13)
    {
        BaseItem * passingCard;
        BaseItem * excemptionCard;
        while (sherlock->getBag()->findItem(PASSING_CARD) && this->wBag->getCount() < 15){
            passingCard = sherlock->getBag()->get(PASSING_CARD);
            this->wBag->insert(passingCard);
        }
    
        while(this->wBag->findItem(EXCEMPTION_CARD) && sherlock->getBag()->getCount() < 13){
            excemptionCard = this->wBag->get(EXCEMPTION_CARD);
            sherlock->getBag()->insert(excemptionCard);
        }
    }
}

//<============================================================================================================================================>

//<=============================================================== CLASS CRIMINAL =============================================================>

Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson) : Character(index, init_pos, map, "Criminal")
{
    this->sherlock = sherlock;
    this->watson = watson;
    this->count = 0;
    this->previous = init_pos;
};

Criminal::~Criminal(){};

Position Criminal::getNextPosition()
{
    Position criminalNext;
    int distance = 0;
    int moveIndex = -1;
    int tmp;
    int indexEnd = -1;
    int sherlockRow = sherlock->getCurrentPosition().getRow();
    int sherlockCol = sherlock->getCurrentPosition().getCol();
    int watsonRow = watson->getCurrentPosition().getRow();
    int watsonCol = watson->getCurrentPosition().getCol();
    Position *nextPosCriminal = new Position[4];

    // Rank: U->L->D->R
    nextPosCriminal[0].setRow(pos.getRow() - 1);
    nextPosCriminal[0].setCol(pos.getCol());

    nextPosCriminal[1].setRow(pos.getRow());
    nextPosCriminal[1].setCol(pos.getCol() - 1);

    nextPosCriminal[2].setRow(pos.getRow() + 1);
    nextPosCriminal[2].setCol(pos.getCol());

    nextPosCriminal[3].setRow(pos.getRow());
    nextPosCriminal[3].setCol(pos.getCol() + 1);

    for (int i = 0; i < 4; i++)
    {
        if (map->isValid(nextPosCriminal[i], this) == false)
        {
            nextPosCriminal[i] = Position::npos;
        }
        else if (nextPosCriminal[i].isEqual(sherlockRow, sherlockCol) || nextPosCriminal[i].isEqual(watsonRow, watsonCol))
        {
            indexEnd = i;
        }
    }

    bool outOfWay = true;

    for (int i = 0; i < 4; i++)
    {
        if (nextPosCriminal[i].isEqual(-1, -1) == false)
        {
            tmp = abs(sherlockRow - nextPosCriminal[i].getRow()) + abs(sherlockCol - nextPosCriminal[i].getCol()) + abs(watsonRow - nextPosCriminal[i].getRow()) + abs(watsonCol - nextPosCriminal[i].getCol());
            if (tmp > distance || (tmp == 0 && outOfWay == true))
            {
                distance = tmp;
                moveIndex = i;
                outOfWay = false;
            }
        }
    }

    if (moveIndex == -1)
    {
        criminalNext = Position::npos;
    }
    else
    {
        criminalNext = nextPosCriminal[moveIndex];
    }
    delete nextPosCriminal;
    return criminalNext;
};

void Criminal::move()
{
    Position criminalNext = this->getNextPosition();
    if (criminalNext.isEqual(-1, -1) == false)
    {
        previous = pos;
        pos.setCol(criminalNext.getCol());
        pos.setRow(criminalNext.getRow());
        count++;
    }
};

string Criminal::str() const
{
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
};

Position Criminal::getPreviousPosition()
{
    return previous;
}

int Criminal::getCount() const
{
    return count;
}
//<============================================================================================================================================>

//<========================================================== CLASS ARRAY MOVING OBJECT =======================================================>

ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    this->count = 0;
    arr_mv_objs = new MovingObject *[capacity];
    for(int i = 0; i < capacity; i++){
        arr_mv_objs[i] = nullptr;
    }
};

ArrayMovingObject::~ArrayMovingObject()
{
    for(int i = 3; i < capacity; i++){
        delete arr_mv_objs[i];
    }
};

bool ArrayMovingObject::isFull() const
{
    return count == capacity;
};

bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if (this->isFull())
    {
        return false;
    }
    else
    {
        int i = mv_obj->getIndex();
        arr_mv_objs[i] = mv_obj;
        count++;
        return true;
    }
};

MovingObject *ArrayMovingObject::get(int i)
{
    return arr_mv_objs[i];
};

string ArrayMovingObject::str() const
{
    string ans = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity);
    if(count == 0){
        ans += ";";
    } else{
        for (int i = 0; i < capacity; i++)
        {
            if(arr_mv_objs[i] == nullptr) continue;
            ans += ";";
            ans += arr_mv_objs[i]->str();
        }
    }
    ans += "]";
    return ans;
}

int ArrayMovingObject::getSize() const
{
    return count;
}

bool ArrayMovingObject::counterEvent(int index)
{
    MovingObject *temp;
    MovingObject *character = arr_mv_objs[index];
    Criminal *cc = dynamic_cast<Criminal *>(arr_mv_objs[0]);
    bool ans = false;
    for (int i = 0; i < count; i++)
    {
        if (i == index)
            continue;
        temp = arr_mv_objs[i];
        if (temp->getCurrentPosition().isEqual(character->getCurrentPosition().getRow(), character->getCurrentPosition().getCol()))
        {
            string nameTemp = temp->getName();
            string nameCharacter = character->getName();
            if (nameCharacter == "Sherlock")
            {
                Sherlock *S = dynamic_cast<Sherlock *>(character);
                if (nameTemp == "Watson")
                {
                    Watson *W = dynamic_cast<Watson *>(temp);
                    S->counterWatson(W);
                    ans = false;
                }
                else if (nameTemp == "Criminal")
                {
                    return true;
                }
                else if (nameTemp == "RobotC")
                {
                    RobotC *c = dynamic_cast<RobotC *>(temp);
                    bool temp = S->counterRobotC(c);
                    if (temp == true)
                    {
                        S->setPos(cc->getCurrentPosition());
                        return true;
                    }
                    else
                    {
                        ans = false;
                    }
                }
                else if (nameTemp == "RobotS")
                {
                    RobotS *s = dynamic_cast<RobotS *>(temp);
                    S->counterRobotS(s);
                    ans = false;
                }
                else if (nameTemp == "RobotW")
                {
                    RobotW *w = dynamic_cast<RobotW *>(temp);
                    S->counterRobotW(w);
                    ans = false;
                }
                else if (nameTemp == "RobotSW")
                {
                    RobotSW *sw = dynamic_cast<RobotSW *>(temp);
                    S->counterRobotSW(sw);
                    ans = false;
                }
            }
            else if (nameCharacter == "Watson")
            {
                Watson *W = dynamic_cast<Watson *>(character);
                if (nameTemp == "Sherlock")
                {
                    Sherlock *S = dynamic_cast<Sherlock *>(temp);
                    W->counterSherlock(S);
                    ans = false;
                }
                else if (nameTemp == "Criminal")
                {
                    return true;
                }
                else if (nameTemp == "RobotC")
                {
                    RobotC *c = dynamic_cast<RobotC *>(temp);
                    W->counterRobotC(c);
                    ans = false;
                }
                else if (nameTemp == "RobotS")
                {
                    RobotS *s = dynamic_cast<RobotS *>(temp);
                    W->counterRobotS(s);
                    ans = false;
                }
                else if (nameTemp == "RobotW")
                {
                    RobotW *w = dynamic_cast<RobotW *>(temp);
                    W->counterRobotW(w);
                    ans = false;
                }
                else if (nameTemp == "RobotSW")
                {
                    RobotSW *sw = dynamic_cast<RobotSW *>(temp);
                    W->counterRobotSW(sw);
                    ans = false;
                }
            }
            else if (nameCharacter == "Criminal")
            {

                Criminal *C = dynamic_cast<Criminal *>(character);
                if (nameTemp == "Sherlock")
                {
                    return true;
                }
                else if (nameTemp == "Watson")
                {
                    return true;
                }
                else if (nameTemp.find("Robot") != string::npos)
                {
                    ans = false;
                }
            }
            else if (nameCharacter == "RobotC")
            {
                RobotC *c = dynamic_cast<RobotC *>(character);
                if (nameTemp.find("Robot") != string::npos)
                {
                    ans = false;
                }
                else if (nameTemp == "Sherlock")
                {
                    Sherlock *S = dynamic_cast<Sherlock *>(temp);
                    bool temp = S->counterRobotC(c);
                    if (temp == true)
                    {
                        S->setPos(cc->getCurrentPosition());
                        return true;
                    }
                    else
                    {
                        ans = false;
                    }
                }
                else if (nameTemp == "Watson")
                {
                    Watson *W = dynamic_cast<Watson *>(temp);
                    W->counterRobotC(c);
                    ans = false;
                }
                else if (nameTemp == "Criminal")
                {
                    ans = false;
                }
            }
            else if (nameCharacter == "RobotS")
            {
                RobotS *s = dynamic_cast<RobotS *>(character);
                if (nameTemp.find("Robot") != string::npos)
                {
                    ans = false;
                }
                else if (nameTemp == "Sherlock")
                {
                    Sherlock *S = dynamic_cast<Sherlock *>(temp);
                    S->counterRobotS(s);
                    ans = false;
                }
                else if (nameTemp == "Watson")
                {
                    Watson *W = dynamic_cast<Watson *>(temp);
                    W->counterRobotS(s);
                    ans = false;
                }
                else if (nameTemp == "Criminal")
                {
                    ans = false;
                }
            }
            else if (nameCharacter == "RobotW")
            {
                RobotW *w = dynamic_cast<RobotW *>(character);
                if (nameTemp.find("Robot") != string::npos)
                {
                    ans = false;
                }
                else if (nameTemp == "Sherlock")
                {
                    Sherlock *S = dynamic_cast<Sherlock *>(temp);
                    S->counterRobotW(w);
                    ans = false;
                }
                else if (nameTemp == "Watson")
                {
                    Watson *W = dynamic_cast<Watson *>(temp);
                    W->counterRobotW(w);
                    ans = false;
                }
                else if (nameTemp == "Criminal")
                {
                    ans = false;
                }
            }
            else if (nameCharacter == "RobotSW")
            {
                RobotSW *sw = dynamic_cast<RobotSW *>(character);
                if (nameTemp.find("Robot") != string::npos)
                {
                    ans = false;
                }
                else if (nameTemp == "Sherlock")
                {
                    Sherlock *S = dynamic_cast<Sherlock *>(temp);
                    S->counterRobotSW(sw);
                    ans = false;
                }
                else if (nameTemp == "Watson")
                {
                    Watson *W = dynamic_cast<Watson *>(temp);
                    W->counterRobotSW(sw);
                    ans = false;
                }
                else if (nameTemp == "Criminal")
                {
                    ans = false;
                }
            }
        }
    }
    return ans;
}

//<============================================================================================================================================>

//<============================================================== CLASS CONFIGURATION =========================================================>

Configuration::Configuration(const string &filepath)
{
    ifstream config(filepath);
    string line;
    int number = 0;
    size_t unfound = string::npos;
    arr_walls = new Position[10000];
    arr_fake_walls = new Position[10000];
    while (getline(config, line))
    {
        if (line.find("MAP_NUM_ROWS") != unfound)
        {
            number = 0;
            for (int i = 13; i < line.length(); i++)
            {
                number = number * 10 + int(line[i] - '0');
            }
            map_num_rows = number;
        }
        else if (line.find("MAP_NUM_COLS") != unfound)
        {
            number = 0;
            for (int i = 13; i < line.length(); i++)
            {
                number = number * 10 + int(line[i] - '0');
            }
            map_num_cols = number;
        }
        else if (line.find("MAX_NUM_MOVING_OBJECTS") != unfound)
        {
            number = 0;
            for (int i = 23; i < line.length(); i++)
            {
                number = number * 10 + int(line[i] - '0');
            }

            max_num_moving_objects = number;
        }
        else if (line.find("ARRAY_WALLS") != unfound)
        {

            if (line.length() == 14)
            {
                num_walls = 0;
            }
            else
            {
                int wallPos = 0;
                int wallIndex = 0;

                for (int i = 14; i < line.length(); i++)
                {
                    if (isdigit(line[i]))
                    {
                        wallPos = wallPos * 10 + int(line[i] - '0');
                    }
                    else if (line[i] == ',')
                    {
                        arr_walls[wallIndex].setRow(wallPos);
                        wallPos = 0;
                    }
                    else if (line[i] == ')')
                    {
                        arr_walls[wallIndex].setCol(wallPos);
                        wallIndex++;
                        wallPos = 0;
                    }
                }
                num_walls = wallIndex;
            }
        }
        else if (line.find("ARRAY_FAKE_WALLS") != unfound)
        {

            if (line.length() == 19)
            {
                num_fake_walls = 0;
            }
            else
            {
                int fakeWallPos = 0;
                int fakeWallIndex = 0;

                for (int i = 14; i < line.length(); i++)
                {
                    if (isdigit(line[i]))
                    {
                        fakeWallPos = fakeWallPos * 10 + int(line[i] - '0');
                    }
                    else if (line[i] == ',')
                    {
                        arr_fake_walls[fakeWallIndex].setRow(fakeWallPos);
                        fakeWallPos = 0;
                    }
                    else if (line[i] == ')')
                    {
                        arr_fake_walls[fakeWallIndex].setCol(fakeWallPos);
                        fakeWallIndex++;
                        fakeWallPos = 0;
                    }
                }
                num_fake_walls = fakeWallIndex;
            }
        }
        else if (line.find("SHERLOCK_MOVING_RULE") != unfound)
        {

            sherlock_moving_rule = line.substr(21, line.length() - 21);
        }
        else if (line.find("SHERLOCK_INIT_POS") != unfound)
        {

            int i = 19;
            int sherlockRow, sherlockCol;
            sherlockRow = 0;
            sherlockCol = 0;
            while (line[i] != ',')
            {
                sherlockRow = sherlockRow * 10 + int(line[i] - '0');
                i++;
            }
            sherlock_init_pos.setRow(sherlockRow);
            i++;
            while (line[i] != ')')
            {
                sherlockCol = sherlockCol * 10 + int(line[i] - '0');
                i++;
            }
            sherlock_init_pos.setCol(sherlockCol);
        }
        else if (line.find("SHERLOCK_INIT_HP") != unfound)
        {

            number = 0;
            for (int i = 17; i < line.length(); i++)
            {
                number = number * 10 + int(line[i] - '0');
            }
            sherlock_init_hp = number;
        }
        else if (line.find("SHERLOCK_INIT_EXP") != unfound)
        {

            number = 0;
            for (int i = 18; i < line.length(); i++)
            {
                number = number * 10 + int(line[i] - '0');
            }
            sherlock_init_exp = number;
        }
        else if (line.find("WATSON_MOVING_RULE") != unfound)
        {

            watson_moving_rule = line.substr(19, line.length() - 19);
        }
        else if (line.find("WATSON_INIT_POS") != unfound)
        {

            int i = 17;
            int watsonRow, watsonCol;
            watsonRow = 0;
            watsonCol = 0;
            while (line[i] != ',')
            {
                watsonRow = watsonRow * 10 + int(line[i] - '0');
                i++;
            }
            watson_init_pos.setRow(watsonRow);
            i++;
            while (line[i] != ')')
            {
                watsonCol = watsonCol * 10 + int(line[i] - '0');
                i++;
            }
            watson_init_pos.setCol(watsonCol);
        }
        else if (line.find("WATSON_INIT_HP") != unfound)
        {

            number = 0;
            for (int i = 15; i < line.length(); i++)
            {
                number = number * 10 + int(line[i] - '0');
            }

            watson_init_hp = number;
        }
        else if (line.find("WATSON_INIT_EXP") != unfound)
        {

            number = 0;
            for (int i = 16; i < line.length(); i++)
            {
                number = number * 10 + int(line[i] - '0');
            }

            watson_init_exp = number;
        }
        else if (line.find("CRIMINAL_INIT_POS") != unfound)
        {

            int i = 19;
            int criminalRow, criminalCol;
            criminalRow = 0;
            criminalCol = 0;
            while (line[i] != ',')
            {
                criminalRow = criminalRow * 10 + int(line[i] - '0');
                i++;
            }
            criminal_init_pos.setRow(criminalRow);
            i++;
            while (line[i] != ')')
            {
                criminalCol = criminalCol * 10 + int(line[i] - '0');
                i++;
            }
            criminal_init_pos.setCol(criminalCol);
        }
        else if (line.find("NUM_STEPS") != unfound)
        {

            number = 0;
            for (int i = 10; i < line.length(); i++)
            {
                number = number * 10 + int(line[i] - '0');
            }
            num_steps = number;
        }
        line.clear();
    }
    config.close();
}

Configuration::~Configuration()
{
    delete[] arr_walls;
    delete[] arr_fake_walls;
}

string Configuration::str() const
{
    string arr_wallsString;
    if (num_walls == 0)
    {
        arr_wallsString = "[]";
    }
    else if (num_walls == 1)
    {
        arr_wallsString = "[";
        arr_wallsString += arr_walls[0].str() + "]";
    }
    else
    {
        arr_wallsString = "[";
        for (int wall = 0; wall < num_walls - 1; wall++)
        {
            arr_wallsString += arr_walls[wall].str();
            arr_wallsString += ";";
        }
        arr_wallsString += arr_walls[num_walls - 1].str() + "]";
    }

    string arr_fake_wallsString;
    if (num_fake_walls == 0)
    {
        arr_fake_wallsString = "[]";
    }
    else if (num_fake_walls == 1)
    {
        arr_fake_wallsString = "[";
        arr_fake_wallsString += arr_fake_walls[0].str() + "]";
    }
    else
    {
        arr_fake_wallsString = "[";
        for (int fakeWall = 0; fakeWall < num_fake_walls - 1; fakeWall++)
        {
            arr_fake_wallsString += arr_fake_walls[fakeWall].str();
            arr_fake_wallsString += ";";
        }
        arr_fake_wallsString += arr_fake_walls[num_fake_walls - 1].str() + "]";
    }

    string ans = "Configuration[\nMAP_NUM_ROWS=" + to_string(map_num_rows) + "\nMAP_NUM_COLS=" + to_string(map_num_cols) + "\nMAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\nNUM_WALLS=" + to_string(num_walls) + "\nARRAY_WALLS=" + arr_wallsString + "\nNUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\nARRAY_FAKE_WALLS=" + arr_fake_wallsString + "\nSHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\nSHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\nSHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\nSHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\nWATSON_MOVING_RULE=" + watson_moving_rule + "\nWATSON_INIT_POS=" + watson_init_pos.str() + "\nWATSON_INIT_HP=" + to_string(watson_init_hp) + "\nWATSON_INIT_EXP=" + to_string(watson_init_exp) + "\nCRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\nNUM_STEPS=" + to_string(num_steps) + "\n]";

    return ans;
}

//<============================================================================================================================================>

//<================================================================= CLASS ROBOT ==============================================================>

Robot::Robot(int index, Position pos, Map *map, RobotType robot_type, Criminal *criminal, const string &name) : MovingObject(index, pos, map, name)
{
    this->robot_type = robot_type;
    this->criminal = criminal;
    int s = getCardinalNumber(pos.getRow() * pos.getCol());
    if (s == 0 || s == 1)
    {
        item = new MagicBook;
    }
    else if (s == 2 || s == 3)
    {
        item = new EnergyDrink;
    }
    else if (s == 4 || s == 5)
    {
        item = new FirstAid;
    }
    else if (s == 6 || s == 7)
    {
        item = new ExcemptionCard;
    }
    else if (s == 8 || s == 9)
    {
        int t = (pos.getRow() * 11 + pos.getCol()) % 4;
        if (t == 0)
        {
            item = new PassingCard("RobotS");
        }
        else if (t == 1)
        {
            item = new PassingCard("RobotC");
        }
        else if (t == 2)
        {
            item = new PassingCard("RobotSW");
        }
        else if (t == 3)
        {
            item = new PassingCard("all");
        }
    }
};

int Robot::getCardinalNumber(int p)
{
    int sum = 0;
    while (p > 0)
    {
        sum += p % 10;
        p = p / 10;
    }
    if (sum < 10)
    {
        return sum;
    }
    return getCardinalNumber(sum);
};

BaseItem *Robot::getItem()
{
    return this->item;
}

Robot *Robot::create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
{
    Robot *ans;
    if (criminal->getCount() == 3)
    {
        RobotC *robot = new RobotC(index, criminal->getPreviousPosition(), map, criminal);
        ans = dynamic_cast<Robot *>(robot);
    }
    else
    {
        int criminalPosRow = criminal->getPreviousPosition().getRow();
        int criminalPosCol = criminal->getPreviousPosition().getCol();
        int sherlockPosRow = sherlock->getCurrentPosition().getRow();
        int sherlockPosCol = sherlock->getCurrentPosition().getCol();
        int watsonPosRow = watson->getCurrentPosition().getRow();
        int watsonPosCol = watson->getCurrentPosition().getCol();
        int distanceToSherlock = abs(criminalPosRow - sherlockPosRow) + abs(criminalPosCol - sherlockPosCol);
        int distanceToWatson = abs(criminalPosRow - watsonPosRow) + abs(criminalPosCol - watsonPosCol);
        if (distanceToSherlock < distanceToWatson)
        {
            RobotS *robot = new RobotS(index, Position(criminalPosRow, criminalPosCol), map, criminal, sherlock);
            ans = dynamic_cast<Robot *>(robot);
        }
        else if (distanceToSherlock > distanceToWatson)
        {
            RobotW *robot = new RobotW(index, Position(criminalPosRow, criminalPosCol), map, criminal, watson);
            ans = dynamic_cast<Robot *>(robot);
        }
        else if (distanceToSherlock == distanceToWatson)
        {
            RobotSW *robot = new RobotSW(index, Position(criminalPosRow, criminalPosCol), map, criminal, sherlock, watson);
            ans = dynamic_cast<Robot *>(robot);
        }
    }
    return ans;
}

RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal) : Robot(index, init_pos, map, C, criminal, "RobotC"){};

RobotC::~RobotC()
{
    delete item;
}

Position RobotC::getNextPosition()
{
    Position nextPos = criminal->getPreviousPosition();
    if(nextPos.isEqual(criminal->getCurrentPosition().getRow(),criminal->getCurrentPosition().getCol())) return Position::npos;
    if (map->isValid(nextPos, this) == true)
    {
        return nextPos;
    }
    return nextPos;
};

void RobotC::move()
{
    Position nextPos = this->getNextPosition();
    if (nextPos.isEqual(-1, -1) == false)
    {
        pos.setRow(nextPos.getRow());
        pos.setCol(nextPos.getCol());
    }
};

int RobotC::getDistance(Character *character) const
{
    int characterCol = character->getCurrentPosition().getCol();
    int characterRow = character->getCurrentPosition().getRow();
    int robotCol = pos.getCol();
    int robotRow = pos.getRow();
    int distance = abs(characterRow - robotRow) + abs(characterCol - robotCol);
    return distance;
};

string RobotC::str() const
{
    string ans = "Robot[pos=" + pos.str() + ";type=C;dist=]";
    return ans;
};

RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *Sherlock) : Robot(index, init_pos, map, S, criminal, "RobotS")
{
    this->sherlock = Sherlock;
};

RobotS::~RobotS()
{
    delete item;
}

Position RobotS::getNextPosition()
{
    Position robotNext;
    int distance = INT_MAX;
    int posIndex = -1;
    int tmp;
    int sherlockRow = sherlock->getCurrentPosition().getRow();
    int sherlockCol = sherlock->getCurrentPosition().getCol();

    Position *arr = new Position[4];
    arr[0].setRow(pos.getRow() - 1);
    arr[0].setCol(pos.getCol());

    arr[1].setRow(pos.getRow());
    arr[1].setCol(pos.getCol() + 1);

    arr[2].setRow(pos.getRow() + 1);
    arr[2].setCol(pos.getCol());

    arr[3].setRow(pos.getRow());
    arr[3].setCol(pos.getCol() - 1);

    for (int i = 0; i < 4; i++)
    {
        if (map->isValid(arr[i], this) == false)
        {
            arr[i] = Position::npos;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (arr[i].isEqual(-1, -1) == false)
        {
            tmp = abs(sherlockRow - arr[i].getRow()) + abs(sherlockCol - arr[i].getCol());
            if (tmp < distance)
            {
                distance = tmp;
                posIndex = i;
            }
        }
    }

    if (posIndex == -1)
    {
        robotNext = Position::npos;
    }
    else
    {
        robotNext = arr[posIndex];
    }

    delete[] arr;
    return robotNext;
}

void RobotS::move()
{
    Position robotSnextPos = this->getNextPosition();
    if (robotSnextPos.isEqual(-1, -1) == false)
    {
        pos.setRow(robotSnextPos.getRow());
        pos.setCol(robotSnextPos.getCol());
    }
}

int RobotS::getDistance() const
{
    int sherlockRow = sherlock->getCurrentPosition().getRow();
    int sherlockCol = sherlock->getCurrentPosition().getCol();
    int robotRow = pos.getRow();
    int robotCol = pos.getCol();
    int distance = abs(sherlockRow - robotRow) + abs(sherlockCol - robotCol);
    return distance;
}

string RobotS::str() const
{
    int distance = this->getDistance();
    string ans = "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(distance) + "]";
    return ans;
}

RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson) : Robot(index, init_pos, map, W, criminal, "RobotW")
{
    this->watson = watson;
};

RobotW::~RobotW()
{
    delete item;
}

Position RobotW::getNextPosition()
{
    Position robotNext;
    int distance = INT_MAX;
    int posIndex = -1;
    int tmp;
    int watsonRow = watson->getCurrentPosition().getRow();
    int watsonCol = watson->getCurrentPosition().getCol();

    Position *arr = new Position[4];
    arr[0].setRow(pos.getRow() - 1);
    arr[0].setCol(pos.getCol());

    arr[1].setRow(pos.getRow());
    arr[1].setCol(pos.getCol() + 1);

    arr[2].setRow(pos.getRow() + 1);
    arr[2].setCol(pos.getCol());

    arr[3].setRow(pos.getRow());
    arr[3].setCol(pos.getCol() - 1);

    for (int i = 0; i < 4; i++)
    {
        if (map->isValid(arr[i], this) == false)
        {
            arr[i] = Position::npos;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (arr[i].isEqual(-1, -1) == false)
        {
            tmp = abs(watsonRow - arr[i].getRow()) + abs(watsonCol - arr[i].getCol());
            if (tmp < distance)
            {
                distance = tmp;
                posIndex = i;
            }
        }
    }

    if (posIndex == -1)
    {
        robotNext = Position::npos;
    }
    else
    {
        robotNext = arr[posIndex];
    }

    delete[] arr;
    return robotNext;
}

void RobotW::move()
{
    Position robotWnextPos = this->getNextPosition();
    if (robotWnextPos.isEqual(-1, -1) == false)
    {
        pos.setRow(robotWnextPos.getRow());
        pos.setCol(robotWnextPos.getCol());
    }
}

int RobotW::getDistance() const
{
    int watsonRow = watson->getCurrentPosition().getRow();
    int watsonCol = watson->getCurrentPosition().getCol();
    int robotRow = pos.getRow();
    int robotCol = pos.getCol();
    int distance = abs(watsonRow - robotRow) + abs(watsonCol - robotCol);
    return distance;
}

string RobotW::str() const
{
    int distance = this->getDistance();
    string ans = "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(distance) + "]";
    return ans;
}

RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson) : Robot(index, init_pos, map, SW, criminal, "RobotSW")
{
    this->sherlock = sherlock;
    this->watson = watson;
};

RobotSW::~RobotSW()
{
    delete item;
}

Position RobotSW::getNextPosition()
{
    Position robotNext;
    int distance = INT_MAX;
    int posIndex = -1;
    int tmp;

    int watsonRow = watson->getCurrentPosition().getRow();
    int watsonCol = watson->getCurrentPosition().getCol();
    int sherlockRow = sherlock->getCurrentPosition().getRow();
    int sherlockCol = sherlock->getCurrentPosition().getCol();

    Position *arr = new Position[8];
    // North
    arr[0].setRow(pos.getRow() - 2);
    arr[0].setCol(pos.getCol());
    // North East
    arr[1].setRow(pos.getRow() - 1);
    arr[1].setCol(pos.getCol() + 1);
    // East
    arr[2].setRow(pos.getRow());
    arr[2].setCol(pos.getCol() + 2);
    // South East
    arr[3].setRow(pos.getRow() + 1);
    arr[3].setCol(pos.getCol() + 1);
    // South
    arr[4].setRow(pos.getRow() + 2);
    arr[4].setCol(pos.getCol());
    // South West
    arr[5].setRow(pos.getRow() + 1);
    arr[5].setCol(pos.getCol() - 1);
    // West
    arr[6].setRow(pos.getRow());
    arr[6].setCol(pos.getCol() - 2);
    // North West
    arr[7].setRow(pos.getRow() - 1);
    arr[7].setCol(pos.getCol() - 1);

    for (int i = 0; i < 8; i++)
    {
        if (map->isValid(arr[i], this) == false)
        {
            arr[i] = Position::npos;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        if (arr[i].isEqual(-1, -1) == false)
        {
            tmp = abs(watsonRow - arr[i].getRow()) + abs(watsonCol - arr[i].getCol()) + abs(sherlockRow - arr[i].getRow()) + abs(sherlockCol - arr[i].getCol());
            if (tmp < distance)
            {
                distance = tmp;
                posIndex = i;
            }
        }
    }

    if (posIndex == -1)
    {
        robotNext = Position::npos;
    }
    else
    {
        robotNext = arr[posIndex];
    }

    delete[] arr;
    return robotNext;
}

void RobotSW::move()
{
    Position robotSWnextPos = this->getNextPosition();
    if (robotSWnextPos.isEqual(-1, -1) == false)
    {
        pos.setRow(robotSWnextPos.getRow());
        pos.setCol(robotSWnextPos.getCol());
    }
}

int RobotSW::getDistance() const
{
    int watsonRow = watson->getCurrentPosition().getRow();
    int watsonCol = watson->getCurrentPosition().getCol();
    int sherlockRow = sherlock->getCurrentPosition().getRow();
    int sherlockCol = sherlock->getCurrentPosition().getCol();
    int robotRow = pos.getRow();
    int robotCol = pos.getCol();
    int distance = abs(watsonRow - robotRow) + abs(watsonCol - robotCol) + abs(sherlockRow - robotRow) + abs(sherlockCol - robotCol);
    return distance;
}

string RobotSW::str() const
{
    int distance = this->getDistance();
    string ans = "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(distance) + "]";
    return ans;
}

//<============================================================================================================================================>

//<================================================================ CLASS BASE ITEM ===========================================================>

ItemType MagicBook::getType() const
{
    return MAGIC_BOOK;
}
string MagicBook::str() const
{
    return "MagicBook";
}

bool MagicBook::canUse(Character *obj, Robot *robot)
{
    if(obj->getName() == "Criminal" || obj->getName().find("Robot") != string::npos) return false;
    if (obj->getExp() <= 350 && robot == nullptr)
    {
        return true;
    }
    return false;
};

void MagicBook::use(Character *obj, Robot *robot)
{

    if (this->canUse(obj, robot) == true)
    {
        obj->setExp((int)(obj->getExp() * 1.25 + 0.9999));
        obj->roundUp();
    }
};

ItemType EnergyDrink::getType() const
{
    return ENERGY_DRINK;
}
string EnergyDrink::str() const
{
    return "EnergyDrink";
}

bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    if(obj->getName() == "Criminal" || obj->getName().find("Robot") != string::npos) return false;
    if (obj->getHp() <= 100 && robot == nullptr)
    {
        return true;
    }
    return false;
};

void EnergyDrink::use(Character *obj, Robot *robot)
{
    if (this->canUse(obj, robot) == true)
    {
        obj->setHp((int)(obj->getHp() * 1.2 + 0.9999));
        obj->roundUp();
    }
};

ItemType FirstAid::getType() const
{
    return FIRST_AID;
}
string FirstAid::str() const
{
    return "FirstAid";
}

bool FirstAid::canUse(Character *obj, Robot *robot)
{
    if(obj->getName() == "Criminal" || obj->getName().find("Robot") != string::npos) return false;
    if ((obj->getExp() <= 350 || obj->getHp() <= 100) && robot == nullptr)
    {
        return true;
    }
    return false;
};

void FirstAid::use(Character *obj, Robot *robot)
{
    if (this->canUse(obj, robot) == true)
    {
        obj->setHp((int)(obj->getHp() * 1.5 + 0.9999));
        obj->roundUp();
    }
};

ItemType ExcemptionCard::getType() const
{
    return EXCEMPTION_CARD;
}

string ExcemptionCard::str() const
{
    return "ExcemptionCard";
}

bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    if (robot == nullptr)
        return false;
    if (obj->getName() == "Sherlock" && obj->getHp() % 2 != 0)
    {
        return true;
    }
    return false;
};

void ExcemptionCard::use(Character *obj, Robot *robot)
{
    if (this->canUse(obj, robot) == true)
    {
        obj->setHp(obj->getHp() * 1);
        obj->setExp(obj->getExp() * 1);
    }
};

ItemType PassingCard::getType() const
{
    return PASSING_CARD;
}
string PassingCard::str() const
{
    return "PassingCard";
}

PassingCard::PassingCard(string challenge)
{
    this->challenge = challenge;
}

bool PassingCard::canUse(Character *obj, Robot *robot)
{
    if (robot == nullptr)
        return false;
    if (obj->getName() == "Watson" && obj->getHp() % 2 == 0)
    {
        return true;
    }
    return false;
};

void PassingCard::use(Character *obj, Robot *robot)
{
    if (this->canUse(obj, robot))
    {
        if (challenge != robot->getName() && challenge != "all")
        {
            obj->setExp(obj->getExp() - 50);
        }
    }
};

string PassingCard::getChallenge()
{
    return this->challenge;
}

//<============================================================================================================================================>

//<================================================================ CLASS BASE BAG ============================================================>

Node::Node(BaseItem *item)
{
    this->item = item;
    this->next = nullptr;
};

BaseBag::BaseBag(Character *character)
{
    this->character = character;
    this->head = nullptr;
    this->count = 0;
};

BaseBag::~BaseBag()
{   
    Node *current = head;
    Node *temp = nullptr;

    current = head;
    while (current != nullptr)
    {
        temp = current->next;
        delete current;
        current = temp;
    }

    head = nullptr;
}

void BaseBag::remove()
{
    Node *temp = head;
    head = head->next;
    delete temp;
    count--;
};

bool BaseBag::insert(BaseItem *item)
{
    if (count == max)
    {
        return false;
    }
    else
    {
        Node *newItem = new Node(item);
        newItem->next = head;
        head = newItem;
        count++;
        return true;
    }
    return 0;
};

BaseItem *BaseBag::get()
{
    Node *current = head;
    BaseItem *found = NULL;
    if (head == nullptr)
        return NULL;
    if (current->item->canUse(character, NULL))
    {
        found = current->item;
        this->remove();
        return found;
    }
    while (current != nullptr)
    {
        if (current->item->canUse(character, NULL))
        {
            BaseItem *temp = head->item;
            head->item = current->item;
            current->item = temp;
            break;
        }
        else
        {
            current = current->next;
        }
    }
    if (current == nullptr)
        return NULL;
    found = head->item;
    this->remove();
    return found;
};

BaseItem *BaseBag::get(ItemType itemType)
{
    Node *current = head;
    BaseItem *found = NULL;
    if (head == nullptr)
        return NULL;
    if (current->item->getType() == itemType)
    {
        found = head->item;
        this->remove();
        return found;
    }
    while (current != nullptr)
    {
        if (current->item->getType() == itemType)
        {
            BaseItem *temp = head->item;
            head->item = current->item;
            current->item = temp;
            break;
        }
        else
        {
            current = current->next;
        }
    }
    if (current == nullptr)
        return NULL;
    found = head->item;
    this->remove();
    return found;
};

string BaseBag::str() const
{
    string ans = "Bag[count=" + to_string(count) + ";";
    if (count != 0)
    {
        Node *current = head;
        ans += current->item->str();
        current = current->next;
        while (current != nullptr)
        {
            ans += ",";
            ans += current->item->str();
            current = current->next;
        }
    }
    ans += "]";
    return ans;
};

int BaseBag::getCount() const{
    return count;
};

bool BaseBag::findItem(ItemType item)
{
    Node *current = head;
    while (current != nullptr)
    {
        if (current->item->getType() == item)
        {
            return true;
        }
        current = current->next;
    }
    return false;
};

SherlockBag::SherlockBag(Sherlock *sherlock) : BaseBag(sherlock)
{
    this->max = 13;
};

WatsonBag::WatsonBag(Watson *watson) : BaseBag(watson)
{
    this->max = 15;
};

//<============================================================================================================================================>

//<====================================================== CLASS STUDYINPINKPROGRAM ============================================================>

StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{

    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

    if (arr_mv_objs->isFull() == false)
    {
        arr_mv_objs->add(criminal);
    }
    if (arr_mv_objs->isFull() == false)
    {
        arr_mv_objs->add(sherlock);
    }
    if (arr_mv_objs->isFull() == false)
    {
        arr_mv_objs->add(watson);
    }
};

StudyPinkProgram::~StudyPinkProgram(){
    delete criminal;
    delete sherlock;
    delete watson;
    delete map;
    delete config;
    delete arr_mv_objs;
};

bool StudyPinkProgram::isStop() const
{
    int sherlockRow = sherlock->getCurrentPosition().getRow();
    int sherlockCol = sherlock->getCurrentPosition().getCol();
    int watsonRow = watson->getCurrentPosition().getRow();
    int watsonCol = watson->getCurrentPosition().getCol();
    if (sherlock->getHp() <= 1 || watson->getHp() <= 1 || criminal->getCurrentPosition().isEqual(sherlockRow, sherlockCol) || criminal->getCurrentPosition().isEqual(watsonRow, watsonCol))
    {
        return true;
    }
    return false;
};

void StudyPinkProgram::printResult() const
{
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
    {
        cout << "Sherlock caught the criminal" << endl;
    }
    else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
    {
        cout << "Watson caught the criminal" << endl;
    }
    else
    {
        cout << "The criminal escaped" << endl;
    }
};

void StudyPinkProgram::printStep(int si) const
{
    cout << "Step: " << setw(4) << setfill('0') << si
         << "--"
         << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
};

void StudyPinkProgram::run(bool verbose)
{
    // Note: This is a sample code. You can change the implementation as you like.
    // TODO
    stop = false;
    for (int istep = 0; istep < config->num_steps && !stop; ++istep)
    {
        int arrMvObjsSize = arr_mv_objs->getSize();
        for (int i = 0; i < arrMvObjsSize; ++i)
        {
            arr_mv_objs->get(i)->move();

            arr_mv_objs->counterEvent(i);

            stop = isStop();

            if (stop)
            {
                printStep(istep);
                break;
            }

            MovingObject *robot = nullptr;
            if (arr_mv_objs->get(i)->getName() == "Criminal")
            {
                robot = Robot::create(arr_mv_objs->getSize(), map, criminal, sherlock, watson);
            }
            if (robot != nullptr)
            {
                if (criminal->getCount() % 3 == 0 && criminal->getCount() > 0)
                {
                    if (arr_mv_objs->add(robot))
                        arrMvObjsSize++;
                }
                else
                {
                    delete robot;
                }
            }

            if (verbose)
            {
                printStep(istep);
            }
        }
    }
    printResult();
};


//<============================================================================================================================================>

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////