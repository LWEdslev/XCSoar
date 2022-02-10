# Build rules for the networking library

LIBNET_SOURCES = \
	$(SRC)/net/AddressInfo.cxx \
	$(SRC)/net/HostParser.cxx \
	$(SRC)/net/Resolver.cxx \
	$(SRC)/net/SocketError.cxx \
	$(SRC)/net/State.cpp \
	$(SRC)/net/ToString.cxx \
	$(SRC)/net/IPv4Address.cxx \
	$(SRC)/net/IPv6Address.cxx \
	$(SRC)/net/StaticSocketAddress.cxx \
	$(SRC)/net/AllocatedSocketAddress.cxx \
	$(SRC)/net/SocketAddress.cxx \
	$(SRC)/net/SocketDescriptor.cxx


# HAVE_WIFI is a creation from August2111 to support the WiFi dialog in general
ifeq ($(HAVE_WIFI),y)
LIBNET_SOURCES += \
	$(SRC)/net/wifi/WPASupplicant.cpp
endif

$(eval $(call link-library,libnet,LIBNET))
