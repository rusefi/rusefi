
# dummy target that forces another to be run once per make invocation
.FORCE:

gitversion.h.gen : .FORCE
	echo "#pragma once" > $@
	echo "#define GIT_HASH \"$$(git rev-parse HEAD)\"" >> $@
	echo "#define GIT_HASH_SHORT \"$$(git rev-parse --short HEAD)\"" >> $@

gitversion.h : gitversion.h.gen
	rsync --checksum $< $@

# All objects could depend on git version
$(OBJS) : gitversion.h
