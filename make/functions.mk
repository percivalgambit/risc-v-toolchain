define binary =
all: $(1)
CLEAN_ITEMS += $(filter-out obj/%,$(1))
$(1): LDFLAGS += $($(build_type)_LDFLAGS)
$(1): $(sort $(call objects,$(2)))
	@mkdir -p $$(@D)
	$$(CXX) $$(LDFLAGS) $$^ -o $$@ $$(LOADLIBES) $$(LDLIBS)
endef

define test =
_test_marker := obj/.test/tested/$(1).tested
test: $$(_test_marker)
$$(_test_marker): $(1).test
	@mkdir -p $$(@D)
	@touch $$@

_test_name := obj/.test/bin/$(1)
.PHONY: $(1).test
$(1).test: $$(_test_name)

$$(_test_name): LDFLAGS += $(debug_LDFLAGS)
$$(_test_name): $(sort $(call debug_objects,src/test_util/catch_main.cc $(2)))
	@mkdir -p $$(@D)
	$$(CXX) $$(LDFLAGS) $$^ -o $$@ $$(LOADLIBES) $$(LDLIBS)
	./$$@
endef
