#!/usr/bin/env python3

from testsupport import subtest, info, warn
from socketsupport import test_server


def main() -> None:
    with subtest("Testing for performance"):
        test_server(num_client_threads=1, messages=20000, timed=False)
        test_server(num_client_threads=2, messages=20000, timed=False)
        test_server(num_client_threads=4, messages=20000, timed=False)
if __name__ == "__main__":
    main()