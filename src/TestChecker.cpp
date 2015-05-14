#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <vector>

#include "Checker.cpp"

Call call(int pid, int handle) {
    Call c;
    c.pid = pid;
    c.handle = handle;
    return c;
}

TEST_CASE("Testing sort_by_procs") {
    set<Call> calls;
    calls.insert(call(1, 10));
    calls.insert(call(1, 5));
    calls.insert(call(0, 2));

    auto procs = sort_by_procs(calls);
    auto proc1 = procs[0];
    auto it = proc1.begin();
    REQUIRE(it != proc1.end());
    REQUIRE((*it) == call(0, 2));
    it++;
    REQUIRE(it == proc1.end());

    auto proc2 = procs[1];
    it = proc2.begin();
    REQUIRE(it != proc2.end());
    REQUIRE((*it) == call(1, 5));
    it++;
    REQUIRE(it != proc2.end());
    REQUIRE((*it) == call(1, 10));
    it++;
    REQUIRE(it == proc2.end());

    auto proc3 = procs[10];
    REQUIRE(proc3.size() == 0);
}

TEST_CASE("regression-1") {
    Process P0(0), P1(1), P2(2);
    // GET THE SECOND PHASE ONCE ALL PROCESSES ARE BLOCKED
    set<Call> trace;
    // P0:
    Call c1 = P0.isend(P1.pid);
    trace.insert(c1);
    Call c3 = P0.wait(c1.handle);
    trace.insert(c3);
    // P1:
    Call c4 = P1.irecv(WILDCARD);
    trace.insert(c4);
    Call c6 = P1.wait(c4.handle);
    trace.insert(c6);
    // P2:
    Call c8 = P2.isend(P1.pid);
    trace.insert(c8);
    Call c9 = P2.wait(c8.handle);
    trace.insert(c9);

    auto tmp = check(trace);
    auto ms = vector<Call>(tmp.begin(), tmp.end());
    REQUIRE(ms.size() == 3);
    REQUIRE(ms[0] == c1);
    REQUIRE(ms[1] == c4);
    REQUIRE(ms[2] == c8);
}
