/*
 *   BSD LICENSE
 *
 *   Copyright(c) 2010-2013 Intel Corporation. All rights reserved.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include "action.h"
#include "vport.h"

#define RTE_LOGTYPE_APP RTE_LOGTYPE_USER1

#define CHECK_NULL(ptr)   do { \
                             if ((ptr) == NULL) return -1; \
                         } while (0)

static action_output(struct rte_mbuf *mbuf, struct action_output *action);

/*
 * Do 'action' of action_type 'type' on 'mbuf'
 */
int action_execute(enum action_type type, void *action, struct rte_mbuf *mbuf)
{
	CHECK_NULL(action);
	CHECK_NULL(mbuf);

	switch (type) {
	case ACTION_OUTPUT:
		action_output(mbuf, action);
		break;
	default:
		printf("action_execute(): action not currently implemented\n");
		return -1;
	}

	return 0;

}

/*
 * Excutes the output action on 'mbuf'
 */
static action_output(struct rte_mbuf *mbuf, struct action_output *action)
{
	uint8_t vport = 0;

	vport = action->port;

	if (IS_PHY_PORT(vport))         /* Physical port */
		send_to_port(vport, mbuf);
	else if (IS_KNI_PORT(vport))    /* KNI FIFO */
		send_to_kni(vport, mbuf);
	else                            /* Client ring */
		send_to_client(vport, mbuf);
}