//
// Created by egi on 5/23/19.
//

#include "axes_grid.h"
#include "text_renderer.h"
#include "cpp/common_funcs.h"

void axes_grid::initialize_gl (QObject *parent)
{
  initializeOpenGLFunctions ();

  program = new QOpenGLShaderProgram (parent);
  if (!program->isLinked ())
  {
    program->addShaderFromSourceFile (QOpenGLShader::Vertex,   ":/shaders/axes_grid.vert");
    program->addShaderFromSourceFile (QOpenGLShader::Fragment, ":/shaders/axes_grid.frag");
    program->link ();
  }

  attribute_coord2d = program->attributeLocation ("coord2d");
  glGenBuffers (1, &vbo_vertices);
}

void axes_grid::prepare (
  unsigned int x_tics_arg,
  unsigned int y_tics_arg,
  float left_x,
  float right_x,
  float bottom_y,
  float top_y)
{
  x_size = (right_x - left_x); y_size = (top_y - bottom_y);
  x_tics = x_tics_arg; y_tics = y_tics_arg;

  const unsigned int points_per_line = 2;
  const unsigned int coords_per_point = 2;
  const unsigned int coords_per_oxs = 2 * x_tics * points_per_line * coords_per_point;
  const unsigned int coords_per_oys = 2 * y_tics * points_per_line * coords_per_point;
  const unsigned int border_coords = 8 * coords_per_point;
  const unsigned int tiks_coords = coords_per_oxs + coords_per_oys;
  total_coords = border_coords + tiks_coords;

  coords.reset ();
  coords = std::make_unique<GLfloat[]> (total_coords);

  coords[tiks_coords + 0]  = left_x;  coords[tiks_coords + 1]  = bottom_y;
  coords[tiks_coords + 2]  = left_x;  coords[tiks_coords + 3]  = top_y;

  coords[tiks_coords + 4]  = left_x;  coords[tiks_coords + 5]  = top_y;
  coords[tiks_coords + 6]  = right_x; coords[tiks_coords + 7]  = top_y;

  coords[tiks_coords + 8]  = right_x; coords[tiks_coords + 9]  = top_y;
  coords[tiks_coords + 10] = right_x; coords[tiks_coords + 11] = bottom_y;

  coords[tiks_coords + 12] = right_x; coords[tiks_coords + 13] = bottom_y;
  coords[tiks_coords + 14] = left_x;  coords[tiks_coords + 15] = bottom_y;

  float dy = (top_y - bottom_y) / (y_tics - 1);
  float *p_coords = coords.get ();

  const bool in = false; /// Tics are inside model
  const short int dir = in ? -1 : 1;

  for (unsigned int y = 0; y < y_tics; y++)
  {
    const float ts = y % long_tic_each == 0 ? long_tic_size : tic_size;

    p_coords[0] = left_x;
    p_coords[1] = bottom_y + dy * y;
    p_coords[2] = left_x - dir * ts;
    p_coords[3] = bottom_y + dy * y;

    p_coords[4] = right_x;
    p_coords[5] = bottom_y + dy * y;
    p_coords[6] = right_x + dir * ts;
    p_coords[7] = bottom_y + dy * y;

    p_coords += 8;
  }

  float dx = (right_x - left_x) / (x_tics - 1);

  for (unsigned int x = 0; x < x_tics; x++)
  {
    const float ts = x % long_tic_each == 0 ? long_tic_size : tic_size;

    p_coords[0] = left_x + dx * x;
    p_coords[1] = bottom_y;
    p_coords[2] = left_x + dx * x;
    p_coords[3] = bottom_y - dir * ts;

    p_coords[4] = left_x + dx * x;
    p_coords[5] = top_y;
    p_coords[6] = left_x + dx * x;
    p_coords[7] = top_y + dir * ts;

    p_coords += 8;
  }

  glBindBuffer (GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * total_coords, coords.get (), GL_DYNAMIC_DRAW);
}

void axes_grid::draw (const QMatrix4x4 &mvp)
{
  auto &tr = text_renderer::instance ();
  cpp_unreferenced (tr);

  program->bind();
  program->setUniformValue ("MVP", mvp);

  glEnableVertexAttribArray (attribute_coord2d);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_vertices);
  glVertexAttribPointer (attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glLineWidth (2.2);
  glDrawArrays (GL_LINES, 0, total_coords);
  glDisableVertexAttribArray (attribute_coord2d);

  program->release ();

  return;

  const float dx = x_size / (x_tics - 1);
  const float dy = y_size / (y_tics - 1);

  const char *format = "%.2e";
  float *p_coords = coords.get ();
  for (unsigned int y = 0; y < y_tics; y+=long_tic_each)
  {
    double number = y * dy;
    int size = std::snprintf (nullptr, 0, format, number);
    std::vector<char> buf (size + 1);
    std::snprintf (buf.data (), buf.size (), format, number);
    tr.render_text (buf.data (), p_coords[2] - long_tic_size / 4, p_coords[3], x_size, mvp, text_renderer::text_anchor::right_center);
    p_coords += 8 * long_tic_each;
  }
  for (unsigned int x = 0; x < x_tics; x+=long_tic_each)
  {
    double number = x * dx;
    int size = std::snprintf (nullptr, 0, format, number);
    std::vector<char> buf (size + 1);
    std::snprintf (buf.data (), buf.size (), format, number);
    tr.render_text (buf.data (), p_coords[2], p_coords[3] - long_tic_size / 4, y_size, mvp, text_renderer::text_anchor::bottom_center);
    p_coords += 8 * long_tic_each;
  }
}
