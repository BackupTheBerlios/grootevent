dtddatadir = $(datadir)/xml/grootevent

dtddata_DATA = grootevent.dtd

EXTRA_DIST = $(dtddata_DATA)

DTD_public_name = "-//DTD grootevent //EN"

install-data-local:
	if [ "xmllint $(sysconfdir)/xml/catalog --noout" == "0" ]; then \
		xmlcatalog --noout --add "public" $(DTD_public_name) \
		$(dtddatadir)/grootevent.dtd $(sysconfdir)/xml/catalog; \
	else \
		mkdir -p $(sysconfdir)/xml/; \
		xmlcatalog --noout --create --add "public" $(DTD_public_name) \
		$(dtddatadir)/grootevent.dtd $(sysconfdir)/xml/catalog; \
	fi

uninstall-local:
	xmlcatalog --noout -del $(DTD_public_name) $(sysconfdir)/xml/catalog;

