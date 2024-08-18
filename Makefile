TARGET = VenomEngine

all: fast_run

debug:
	bazel build //:$(TARGET) --compilation_mode=dbg

release:
	bazel build //:$(TARGET) --compilation_mode=opt

debug_run: debug
	bazel run //:$(TARGET) --compilation_mode=dbg

release_run: release
	bazel run //:$(TARGET) --compilation_mode=opt

fast:
	bazel build //:$(TARGET) --compilation_mode=fastbuild

fast_run: fast
	bazel run //:$(TARGET) --compilation_mode=fastbuild

clean:
	bazel clean

.PHONY: debug release debug_run release_run fast fast_run clean