#!/usr/bin/env python3

from testsupport import subtest
from socketsupport import test_server


def main() -> None:
    with subtest("Testing single-threaded server with single-threaded client"):
        test_server(num_client_threads=1, messages=80000, timed=False)


if __name__ == "__main__":
    main()