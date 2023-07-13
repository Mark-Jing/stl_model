
#ifndef DICE_H
#define DICE_H

#include "model.h"


class Dice : public Model {
public:
    Dice();

    virtual void init() override;
    virtual void update() override;
    virtual void paint(const QMatrix4x4 &projection, const QMatrix4x4 &view) override;

    void fill_in_vertices();
private:
    float *m_vertexBuffer = nullptr;
    int m_vertexCount = 0;
};

#endif // DICE_H
