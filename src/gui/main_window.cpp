//
// Created by egi on 5/11/19.
//

#include <QtWidgets>

#include "main_window.h"
#include "gui/opengl_widget.h"

main_window::main_window(unsigned int nx, unsigned int ny, float x_size, float y_size)
  : gl (new opengl_widget (nx, ny, x_size, y_size))
{
  // Set OpenGL Version information
  // Note: This format must be set before show() is called.
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);

  gl->setFormat(format);

  setCentralWidget (gl);
}

main_window::~main_window() = default;