#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include "gplot.h"

int func(double t, const double y[], double f[], void *params)
{
  double mu = *(double *)params;
  f[0] = y[1];
  f[1] = -y[0] - mu * y[1] * (y[0] * y[0] - 1);
  return GSL_SUCCESS;
}

int jac(double t, const double y[], double *dfdy, double dfdt[], void *params)
{
  double mu = *(double *)params;

  gsl_matrix_view dfdy_mat = gsl_matrix_view_array(dfdy, 2, 2);
  gsl_matrix *m = &dfdy_mat.matrix;

  gsl_matrix_set(m, 0, 0, 0.0);
  gsl_matrix_set(m, 0, 1, 1.0);
  gsl_matrix_set(m, 1, 0, -2.0 * mu * y[0] * y[1] - 1.0);
  gsl_matrix_set(m, 1, 1, -mu * (y[0] * y[0] - 1.0));

  dfdt[0] = 0.0;
  dfdt[1] = 0.0;
  return GSL_SUCCESS;
}

int main(int argc, char const *argv[])
{
  if (argc != 2) {
    printf("Usage: %s output_file_name\n", argv[0]);
    return 1;
  }

  FILE *fp;
  double mu = 10;

  gsl_odeiv2_system sys = {func, jac, 2, &mu};

  gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk8pd, 1e-6, 1e-6, 0.0);

  int i;
  double t = 0.0, t1 = 100.0;
  double y[2] = {1.0, 0.0};

  fp = fopen("vanderpol.dat", "w");

  for (i = 1; i <= 100; i++)
  {
    double ti = i * t1 / 100.0;
    int status = gsl_odeiv2_driver_apply(d, &t, ti, y);
    if (status != GSL_SUCCESS)
    {
      fprintf(stderr, "GSL ODE driver error: %d\n", status);
      break;
    }

    fprintf(fp, "%.5e %.5e %.5e\n", t, y[0], y[1]);
  }

  fclose(fp);
  fp = NULL;
  gsl_odeiv2_driver_free(d);

  gplot_t *g = gplot_init();
  gplot_cmd(g, "set terminal png");
  char cmd[100];
  sprintf(cmd, "set output '%s'", argv[1]);
  gplot_cmd(g, cmd);

  gplot_title(g, "My function");
  gplot_xlabel(g, "x [div]");
  gplot_ylabel(g, "Signal [div]");
  gplot_style(g, "line 1 lt 1 lw 2 pt 6 ps 0.7 lc rgb 'blue'");

  gplot_plot_dat(g, "vanderpol.dat", "1:2 w linespoints ls 1");

  gplot_free(g);

  return 0;
}