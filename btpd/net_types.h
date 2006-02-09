#ifndef BTPD_NET_TYPES_H
#define BTPD_NET_TYPES_H

BTPDQ_HEAD(net_tq, net);
BTPDQ_HEAD(peer_tq, peer);
BTPDQ_HEAD(piece_tq, piece);
BTPDQ_HEAD(block_request_tq, block_request);

struct net {
    struct torrent *tp;

    int active;
    int endgame;

    uint8_t *busy_field;
    uint32_t npcs_busy;
    unsigned *piece_count;
    struct piece_tq getlst;

    unsigned long rate_up, rate_dwn;
    unsigned long long uploaded, downloaded;

    unsigned npeers;
    struct peer_tq peers;

    BTPDQ_ENTRY(net) entry;
};

enum input_state {
    SHAKE_PSTR,
    SHAKE_INFO,
    SHAKE_ID,
    BTP_MSGSIZE,
    BTP_MSGHEAD,
    BTP_PIECEMETA,
    BTP_MSGBODY
};

struct peer {
    int sd;
    uint16_t flags;
    uint8_t *piece_field;
    uint32_t npieces;
    uint32_t nwant;

    uint8_t id[20];

    struct net *n;

    struct block_request_tq my_reqs;

    unsigned nreqs_out;
    unsigned npiece_msgs;

    size_t outq_off;
    struct nb_tq outq;

    struct event in_ev;
    struct event out_ev;

    unsigned long rate_up, rate_dwn;
    unsigned long count_up, count_dwn;

    struct {
        uint32_t msg_len;
        uint8_t msg_num;
        uint32_t pc_index;
        uint32_t pc_begin;
        enum input_state state;
        size_t st_bytes;
        char *buf;
        size_t off;
    } in;

    BTPDQ_ENTRY(peer) p_entry;
    BTPDQ_ENTRY(peer) ul_entry;
    BTPDQ_ENTRY(peer) rq_entry;
    BTPDQ_ENTRY(peer) wq_entry;
};

struct piece {
    struct net *n;

    uint32_t index;

    unsigned nreqs;

    unsigned nblocks;
    unsigned ngot;
    unsigned nbusy;
    unsigned next_block;

    struct block *blocks;

    const uint8_t *have_field;
    uint8_t *down_field;

    BTPDQ_ENTRY(piece) entry;
};

struct block {
    struct piece *pc;
    struct net_buf *msg;
    struct block_request_tq reqs;
};

struct block_request {
    struct peer *p;
    struct block *blk;
    BTPDQ_ENTRY(block_request) p_entry;
    BTPDQ_ENTRY(block_request) blk_entry;
};

#endif
