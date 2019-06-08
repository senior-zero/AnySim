//
// Created by egi on 5/12/19.
//

#ifndef ANYSIM_GUI_SIMULATION_MANAGER_H
#define ANYSIM_GUI_SIMULATION_MANAGER_H

class project_manager;

class gui_simulation_manager
{
public:
  gui_simulation_manager (project_manager &pm, int argc_arg, char *argv_arg[]);

  int run ();

private:
  project_manager &pm;
  int argc;
  char **argv;
};

#endif //ANYSIM_GUI_SIMULATION_MANAGER_H
