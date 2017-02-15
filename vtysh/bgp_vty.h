/* BGP CLI implementation with openswitch vtysh.
 *
 * Copyright (C) 1997, 98 Kunihiro Ishiguro
 * Copyright (C) 2015 Hewlett Packard Enterprise Development LP
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * File: bgp_vty.h
 *
 * Purpose: This file contains function declarations of BGP CLI.
 */
#ifndef _BGP_VTY_H
#define _BGP_VTY_H

#include "openswitch-idl.h"
#include "ovsdb-idl.h"
#include "vtysh/vtysh_ovsdb_config.h"
#include "vtysh/vtysh_ovsdb_if.h"

#define CMD_AS_RANGE "<1-4294967295>"
#define NETWORK_MAX_LEN 49
#define TIMER_KEY_MAX_LENGTH 80
#define BGP_MAX_TIMERS 2
#define BGP_DEFAULT_HOLDTIME 180
#define BGP_DEFAULT_KEEPALIVE 60

/* BGP error codes.  */
#define BGP_ERR_NO_VRF_FOUND                        "VRF is not found"
#define BGP_ERR_ROUTER_IS_NOT_CONFIGURED            "BGP router is not configured"
#define BGP_ERR_NEIGHBOR_IS_NOT_CONFIGURED          "Neighbor is not configured"
#define BGP_ERR_COMMAND_EXIST                       "Command exists"
#define BGP_ERR_COMMAND_NOT_EXIST                   "Command does not exist"
#define BGP_ERR_PEER_GROUP_IS_NOT_CREATED           "Peer group is not exist. Create the peer-group first"
#define BGP_ERR_PEER_GROUP_ALREADY_CREATED          "Peer group already exists"
#define BGP_ERR_NEIGHBOR_CREATION_FAILED            "BGP neighbor/peer group object creation failed"
#define BGP_ERR_NEIGHBOR_NOT_IN_PEER_GROUP          "Neighbor is not in a peer group"
#define BGP_ERR_UNABLE_TO_SET_TIMERS                "Unable to set timers. Neighbor already has been assigned to the peer group"
#define BGP_ERR_UNSPECIFIED_REMOTE_AS               "Specify peer remote AS or peer-group remote AS first"
#define BGP_ERR_REMOVE_PRIVATE_AS                   "Private AS cannot be removed for IBGP peers"
#define BGP_ERR_NO_PREFIX_LIST                      "Prefix List is not found"
#define BGP_ERR_NO_PREFIX_LIST_WITH_DESCR           "Prefix List with the given description is not found"
#define BGP_ERR_NO_PREFIX_LIST_ENTRY                "Prefix List entry is not found"
#define BGP_ERR_NO_AS_PATH_FILTER                   "AS-Path filter not found"
#define BGP_ERR_NO_ROUTE_MAP                        "Route-map not found"
#define BGP_ERR_NO_ROUTE_MAP_ENTRY                  "Route Map entry not found"
#define BGP_ERR_NO_COMMUNITY_FILTER_LIST            "Community-filter list not found"
#define BGP_ERR_EBGP_MULTIHOP_TTL_SECURITY_CONFIG   "Can't configure ebgp-multihop and ttl-security at the same time"

/*
** depending on the outcome of the db transaction, returns
** the appropriate value for the cli command execution.
*/
inline static int
cli_command_result (enum ovsdb_idl_txn_status status)
{
    if ((status == TXN_SUCCESS) || (status == TXN_UNCHANGED)) {
        return CMD_SUCCESS;
    }
    return CMD_WARNING;
}
/********************** standard database txn operations ***********************/

#define START_DB_TXN(txn)                                       \
    do {                                                        \
        txn = cli_do_config_start();                            \
        if (txn == NULL) {                                      \
            vty_out(vty, "OVSDB create transaction failed%s", VTY_NEWLINE);  \
            cli_do_config_abort(txn);                                        \
            return CMD_OVSDB_FAILURE;                                        \
        }                                                                    \
    } while (0)

#define END_DB_TXN(txn)                                   \
    do {                                                  \
        enum ovsdb_idl_txn_status status;                 \
        status = cli_do_config_finish(txn);               \
        return cli_command_result(status);                \
    } while (0)

#define ERRONEOUS_DB_TXN(txn, error_message)                        \
    do {                                                            \
        cli_do_config_abort(txn);                                   \
        vty_out(vty, "%s%s", error_message, VTY_NEWLINE);           \
        return CMD_WARNING;                                         \
    } while (0)

/* used when NO error is detected but still need to terminate */
#define ABORT_DB_TXN(txn, message)                             \
    do {                                                       \
        cli_do_config_abort(txn);                                   \
        vty_out(vty, "%s%s", message, VTY_NEWLINE);                 \
        return CMD_SUCCESS;                                         \
    } while (0)

/********************** helper routines which find things ***********************/

void bgp_vty_init (void);
void policy_vty_init(void);

#endif /* _BGP_VTY_H */
