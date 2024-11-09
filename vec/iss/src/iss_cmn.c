#include <iss_cmn.h>

int iss_pps_fget(struct iss_fio* f, struct iss_pps* pps) {
  fscanf(f->pps, "%lf", &pps->eps);
  fscanf(f->pps, "%d", &pps->mk);

  return 0;
}

void iss_fio_close(struct iss_fio* f) {
  fclose(f->pps);
  fclose(f->x);
  fclose(f->f);
}
