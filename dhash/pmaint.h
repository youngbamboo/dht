#ifndef __PMAINT_H__
#define __PMAINT_H__

#include <chord.h>
#include <chord_prot.h>

#include <dhash_common.h>
#include <dhash.h>
#include <dhashcli.h>
#include <ihash.h>
#include <location.h>

typedef callback<void, ref<dbrec> >::ref delete_t;


class pmaint {

public:
  pmaint (dhashcli *cli, ptr<vnode> host_node, ptr<dbfe> db, 
	  delete_t delete_helper);

  void start ();
  void stop ();

  enum { PRTTMTINY = 1, PRTTMSHORT = 10, PRTTMLONG = 60, MAX_PENDING = 20};
  enum { PMAINT_HANDOFF_ERROR = 0, PMAINT_HANDOFF_NOTPRESENT = 1, 
	 PMAINT_HANDOFF_PRESENT = -1};

  static bigint db_next (ptr<dbfe> db, bigint a);
  static vec<bigint> get_keys (ptr<dbfe> db, bigint a, bigint b, 
			       u_int maxcount);

private:

  //helpers from parent class
  dhashcli *cli;
  ptr<vnode> host_node;
  ptr<dbfe> db;
  delete_t delete_helper;

  //state
  bool pmaint_searching;
  bigint pmaint_next_key;

  timecb_t *active_cb;

  void pmaint_next ();
  void pmaint_lookup (chordID key, dhash_stat err, vec<chord_node> sl, route r);
  void pmaint_offer (bigint key, chord_node succ);
  void pmaint_offer_cb (chord_node dst, bigint key, ref<dhash_offer_res> res, 
			clnt_stat err);
  void pmaint_handoff (chord_node dst, bigint key, cbi cb);
  void pmaint_handoff_cb (bigint key, cbi cb, dhash_stat err, bool present);
  void handed_off_cb (bigint key, int status);
};

#endif