/*
 *	BIRD -- Direct Device Routes
 *
 *	(c) 1998--2000 Martin Mares <mj@ucw.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

/**
 * DOC: Direct
 *
 * The Direct protocol works by converting all ifa_notify() events it receives
 * to rte_update() calls for the corresponding network.
 */

#undef LOCAL_DEBUG

#include "nest/bird.h"
#include "nest/iface.h"
#include "nest/protocol.h"
#include "nest/route.h"
#include "nest/rt-dev.h"
#include "conf/conf.h"
#include "lib/resource.h"
#include "lib/string.h"

static void
dev_ifa_notify(struct proto *p, unsigned c, struct ifa *ad)
{
  struct rt_dev_config *P = (void *) p->cf;

  if (!EMPTY_LIST(P->iface_list) &&
      !iface_patt_find(&P->iface_list, ad->iface))
    /* Empty list is automagically treated as "*" */
    return;
  if (c & IF_CHANGE_DOWN)
    {
      net *n;

      DBG("dev_if_notify: %s:%I going down\n", ad->iface->name, ad->ip);
      n = net_find(p->table, ad->prefix, ad->pxlen);
      if (!n)
	{
	  DBG("dev_if_notify: device shutdown: prefix not found\n");
	  return;
	}
      rte_update(p->table, n, p, p, NULL);
    }
  else if (c & IF_CHANGE_UP)
    {
      rta *a, A;
      net *n;
      rte *e;

      DBG("dev_if_notify: %s:%I going up\n", ad->iface->name, ad->ip);
      bzero(&A, sizeof(A));
      A.proto = p;
      A.source = RTS_DEVICE;
      A.scope = ad->scope;
      A.cast = RTC_UNICAST;
      A.dest = RTD_DEVICE;
      A.iface = ad->iface;
      A.eattrs = NULL;
      a = rta_lookup(&A);
      n = net_get(p->table, ad->prefix, ad->pxlen);
      e = rte_get_temp(a);
      e->net = n;
      e->pflags = 0;
      rte_update(p->table, n, p, p, e);
    }
}

static struct proto *
dev_init(struct proto_config *c)
{
  struct proto *p = proto_new(c, sizeof(struct proto));

  p->ifa_notify = dev_ifa_notify;
  p->min_scope = SCOPE_HOST;
  return p;
}

static int
dev_reconfigure(struct proto *p, struct proto_config *new)
{
  struct rt_dev_config *o = (struct rt_dev_config *) p->cf;
  struct rt_dev_config *n = (struct rt_dev_config *) new;
  
  return iface_patts_equal(&o->iface_list, &n->iface_list, NULL);
}

struct protocol proto_device = {
  name:		"Direct",
  template:	"direct%d",
  init:		dev_init,
  reconfigure:	dev_reconfigure
};
