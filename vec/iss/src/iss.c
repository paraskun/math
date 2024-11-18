#include <iss.h>

int iss_pkt_cls(struct iss_pkt* pkt) {
  if (pkt->pps)
    fclose(pkt->pps);

  if (pkt->x)
    fclose(pkt->x);

  if (pkt->f)
    fclose(pkt->f);

  return 0;
}
