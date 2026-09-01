#!/usr/bin/env bash

set -u
set -o pipefail

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_DIR="$ROOT_DIR/minitalk"
TMP_DIR=$(mktemp -d "${TMPDIR:-/tmp}/42minitalk-regression.XXXXXX")

SERVER_PID=""
TESTS_RUN=0
TESTS_PASSED=0

fail()
{
    printf 'FAIL: %s\n' "$1" >&2
    exit 1
}

pass()
{
    TESTS_RUN=$((TESTS_RUN + 1))
    TESTS_PASSED=$((TESTS_PASSED + 1))
    printf 'PASS: %s\n' "$1"
}

stop_server()
{
    if [ -n "$SERVER_PID" ] && kill -0 "$SERVER_PID" 2>/dev/null; then
        kill "$SERVER_PID" 2>/dev/null || true
        wait "$SERVER_PID" 2>/dev/null || true
    fi
    SERVER_PID=""
}

cleanup()
{
    stop_server
    make -C "$BUILD_DIR" fclean >/dev/null 2>&1 || true
    rm -rf "$TMP_DIR"
}

trap cleanup EXIT INT TERM

wait_for_server()
{
    log_file=$1
    attempts=0

    while [ "$attempts" -lt 100 ]; do
        if grep -q '^Server PID is ' "$log_file" 2>/dev/null; then
            return 0
        fi

        if ! kill -0 "$SERVER_PID" 2>/dev/null; then
            return 1
        fi

        sleep 0.05
        attempts=$((attempts + 1))
    done

    return 1
}

start_server()
{
    mode=$1
    server_log=$2
    server_err=$3

    stop_server

    : > "$server_log"
    : > "$server_err"

    "$BUILD_DIR/server" >"$server_log" 2>"$server_err" &
    SERVER_PID=$!

    if ! wait_for_server "$server_log"; then
        printf '%s server failed to become ready.\n' "$mode" >&2
        printf '%s\n' '--- stdout ---' >&2
        cat "$server_log" >&2 || true
        printf '%s\n' '--- stderr ---' >&2
        cat "$server_err" >&2 || true
        fail "$mode server startup"
    fi

    if ! grep -qx "Server PID is $SERVER_PID" "$server_log"; then
        printf 'Expected PID banner: Server PID is %s\n' "$SERVER_PID" >&2
        cat "$server_log" >&2 || true
        fail "$mode server PID banner"
    fi

    pass "$mode server startup"
    pass "$mode server PID banner"
}

run_client()
{
    mode=$1
    message=$2
    stdout_file=$3
    stderr_file=$4

    : > "$stdout_file"
    : > "$stderr_file"

    if ! timeout 5s \
        "$BUILD_DIR/client" \
        "$SERVER_PID" \
        "$message" \
        >"$stdout_file" \
        2>"$stderr_file"; then
        printf '%s client failed or timed out for message: <%s>\n' \
            "$mode" "$message" >&2
        printf '%s\n' '--- stdout ---' >&2
        cat "$stdout_file" >&2 || true
        printf '%s\n' '--- stderr ---' >&2
        cat "$stderr_file" >&2 || true
        fail "$mode client transfer"
    fi

    if [ -s "$stderr_file" ]; then
        printf '%s client produced unexpected stderr:\n' "$mode" >&2
        cat "$stderr_file" >&2
        fail "$mode client stderr"
    fi

    if ! kill -0 "$SERVER_PID" 2>/dev/null; then
        fail "$mode server remained alive"
    fi
}

assert_file_equals()
{
    expected=$1
    actual=$2
    description=$3

    if ! cmp -s "$expected" "$actual"; then
        printf 'Output mismatch: %s\n' "$description" >&2
        printf '%s\n' '--- expected ---' >&2
        cat -A "$expected" >&2 || true
        printf '%s\n' '--- actual ---' >&2
        cat -A "$actual" >&2 || true
        fail "$description"
    fi

    pass "$description"
}

run_mandatory()
{
    printf '\n=== MANDATORY REGRESSION ===\n'

    make -C "$BUILD_DIR" fclean >/dev/null 2>&1 \
        || fail "mandatory pre-clean"

    make -C "$BUILD_DIR" >/dev/null 2>&1 \
        || fail "mandatory build"

    pass "mandatory build"

    server_log="$TMP_DIR/mandatory-server.out"
    server_err="$TMP_DIR/mandatory-server.err"
    client_out="$TMP_DIR/mandatory-client.out"
    client_err="$TMP_DIR/mandatory-client.err"
    expected="$TMP_DIR/mandatory-expected.out"
    payload="$TMP_DIR/mandatory-payload.out"

    start_server "mandatory" "$server_log" "$server_err"

    run_client \
        "mandatory" \
        "Hello 42" \
        "$client_out" \
        "$client_err"

    if [ -s "$client_out" ]; then
        cat "$client_out" >&2
        fail "mandatory client stdout remains empty"
    fi
    pass "mandatory client stdout remains empty"

    run_client \
        "mandatory" \
        "" \
        "$client_out" \
        "$client_err"

    if [ -s "$client_out" ]; then
        cat "$client_out" >&2
        fail "mandatory empty-message client stdout remains empty"
    fi
    pass "mandatory empty-message client stdout remains empty"

    run_client \
        "mandatory" \
        "Second message" \
        "$client_out" \
        "$client_err"

    if [ -s "$client_out" ]; then
        cat "$client_out" >&2
        fail "mandatory repeated client stdout remains empty"
    fi
    pass "mandatory repeated client stdout remains empty"

    tail -n +2 "$server_log" > "$payload"
    printf 'Hello 42\n\nSecond message\n' > "$expected"

    assert_file_equals \
        "$expected" \
        "$payload" \
        "mandatory received payload sequence"

    if [ -s "$server_err" ]; then
        printf 'Mandatory server produced unexpected stderr:\n' >&2
        cat "$server_err" >&2
        fail "mandatory server stderr"
    fi
    pass "mandatory server stderr remains empty"

    if kill -0 "$SERVER_PID" 2>/dev/null; then
        pass "mandatory server remains available after repeated clients"
    else
        fail "mandatory server remains available after repeated clients"
    fi

    stop_server
}

run_bonus()
{
    printf '\n=== BONUS REGRESSION ===\n'

    make -C "$BUILD_DIR" fclean >/dev/null 2>&1 \
        || fail "bonus pre-clean"

    make -C "$BUILD_DIR" bonus >/dev/null 2>&1 \
        || fail "bonus build"

    pass "bonus build"

    server_log="$TMP_DIR/bonus-server.out"
    server_err="$TMP_DIR/bonus-server.err"
    client_out="$TMP_DIR/bonus-client.out"
    client_err="$TMP_DIR/bonus-client.err"
    expected="$TMP_DIR/bonus-expected.out"
    payload="$TMP_DIR/bonus-payload.out"
    ack_expected="$TMP_DIR/bonus-ack-expected.out"

    start_server "bonus" "$server_log" "$server_err"

    run_client \
        "bonus" \
        "Bonus ACK" \
        "$client_out" \
        "$client_err"

    printf 'Message delivered to server.\n' > "$ack_expected"

    assert_file_equals \
        "$ack_expected" \
        "$client_out" \
        "bonus delivery acknowledgement"

    run_client \
        "bonus" \
        "" \
        "$client_out" \
        "$client_err"

    assert_file_equals \
        "$ack_expected" \
        "$client_out" \
        "bonus empty-message acknowledgement"

    tail -n +2 "$server_log" > "$payload"
    printf 'Bonus ACK\n\n' > "$expected"

    assert_file_equals \
        "$expected" \
        "$payload" \
        "bonus received payload sequence"

    if [ -s "$server_err" ]; then
        printf 'Bonus server produced unexpected stderr:\n' >&2
        cat "$server_err" >&2
        fail "bonus server stderr"
    fi
    pass "bonus server stderr remains empty"

    if kill -0 "$SERVER_PID" 2>/dev/null; then
        pass "bonus server remains available after repeated clients"
    else
        fail "bonus server remains available after repeated clients"
    fi

    stop_server
}

printf 'Minitalk regression validation\n'
printf 'Repository: %s\n' "$ROOT_DIR"

run_mandatory
run_bonus

printf '\n=== REGRESSION SUMMARY ===\n'
printf 'Passed: %d/%d\n' "$TESTS_PASSED" "$TESTS_RUN"

if [ "$TESTS_RUN" -ne "$TESTS_PASSED" ]; then
    fail "regression suite"
fi

printf 'PASS: all runtime regressions\n'
