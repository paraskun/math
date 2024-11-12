#include <iss.h>

int iss_pps_fget(struct iss_pkt* pkt, struct iss_pps* pps) {
  fscanf(pkt->pps, "%lf", &pps->eps);
  fscanf(pkt->pps, "%d", &pps->mk);

  return 0;
}

int iss_pkt_close(struct iss_pkt* pkt) {
  fclose(pkt->pps);
  fclose(pkt->x);
  fclose(pkt->f);

  return 0;
}
