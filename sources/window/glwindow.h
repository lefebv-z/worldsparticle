#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QWindow>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_2_Core>
#include <QKeyEvent>
#include "graphic/myglpainter.h"
#include "model/model.h"
#include <QTime>

class GLWindow : public QWindow
{
    Q_OBJECT
    public:
        explicit GLWindow(QWindow *parent = 0);
        ~GLWindow();

public:
    static QOpenGLContext *context;
    static QSurface      *surface;
    static QOpenGLFunctions_4_2_Core *m_funcs;

    void    start(Model *model);

protected:
    virtual void    keyPressEvent(QKeyEvent *e);

private:
        QOpenGLContext _context;
        QSurfaceFormat _format;
};

#endif // GLWINDOW_H
