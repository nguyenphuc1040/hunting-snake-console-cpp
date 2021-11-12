#pragma once

class Position2D {
    public:
        short x;
        short y;
        Position2D();
        Position2D(short, short);
        void SetXY(short, short);
        Position2D& operator += (const Position2D&);
        bool operator == (Position2D) const;
};

Position2D::Position2D() { 
}
Position2D::Position2D(short x, short y){
    this->x = x;
    this->y = y;
}
Position2D& Position2D::operator += (const Position2D& obj){
    this->x += obj.x;
    this->y += obj.y;
    return *this;
}
void Position2D::SetXY(short x, short y) {
    this->x = x;
    this->y = y;
}

bool Position2D::operator == (Position2D obj) const {
    return (this->x == obj.x && this->y == obj.y);
}
