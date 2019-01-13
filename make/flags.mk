define add_flag =
  $(eval
    ifeq ($(origin $(1)),undefined)
      $(1) := $(2)
    endif
  )
endef

