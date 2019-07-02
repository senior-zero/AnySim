#ifndef EULER_2D_INTERFACE_H_
#define EULER_2D_INTERFACE_H_

template <class float_type>
float_type euler_2d_calculate_dt_gpu_interface (
  unsigned int n_cells,
  float_type gamma,
  float_type cfl,
  float_type min_len,
  float_type *workspace,
  const float_type *p_rho,
  const float_type *p_u,
  const float_type *p_v,
  const float_type *p_p);

template <class float_type>
void euler_2d_calculate_next_time_step_gpu_interface (
    unsigned int nx,
    unsigned int ny,
    float_type dt,
    float_type gamma,
    float_type cell_area,
    const float_type *edge_lengths,
    const float_type *p_rho,
    float_type *p_rho_next,
    const float_type *p_u,
    float_type *p_u_next,
    const float_type *p_v,
    float_type *p_v_next,
    const float_type *p_p,
    float_type *p_p_next);

#endif
