$(call add_flag,build_type,release)

objects = $(patsubst src/%.cc,obj/$(build_type)/%.o,$(sort $(1)))
release_objects = $(patsubst src/%.cc,obj/release/%.o,$(sort $(1)))
debug_objects = $(patsubst src/%.cc,obj/debug/%.o,$(sort $(1)))

obj/debug/%.o: CPPFLAGS += $(debug_CPPFLAGS)
obj/debug/%.o: CXXFLAGS += $(debug_CXXFLAGS)
obj/debug/%.o: src/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

obj/release/%.o: CPPFLAGS += $(release_CPPFLAGS)
obj/release/%.o: CXXFLAGS += $(release_CXXFLAGS)
obj/release/%.o: src/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

obj/release/%.d: src/%.cc
obj/debug/%.d: src/%.cc
-include $(shell find obj/ -type f -name "*.d")
