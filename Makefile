default: desktop-exec lsapp zg-report-app application.conf application-click.conf application-legacy.conf
	@echo "Building"

desktop-exec: desktop-exec.c
	gcc -o desktop-exec desktop-exec.c `pkg-config --cflags --libs glib-2.0`

lsapp: lsapp.c
	gcc -o lsapp lsapp.c `pkg-config --cflags --libs gio-2.0`

zg-report-app: zg-report-app.c
	gcc -o zg-report-app zg-report-app.c `pkg-config --cflags --libs zeitgeist-1.0`

application-legacy.conf: application-legacy.conf.in
	sed -e "s|\@libexecdir\@|/usr/lib/$(DEB_BUILD_MULTIARCH)/upstart-app-launch/|" application-legacy.conf.in > application-legacy.conf

install: application-legacy.conf desktop-exec
	mkdir -p $(DESTDIR)/usr/share/upstart/sessions
	install -m 644 application.conf $(DESTDIR)/usr/share/upstart/sessions/
	install -m 644 application-legacy.conf $(DESTDIR)/usr/share/upstart/sessions/
	install -m 644 application-click.conf $(DESTDIR)/usr/share/upstart/sessions/
	mkdir -p $(DESTDIR)/usr/lib/$(DEB_BUILD_MULTIARCH)/upstart-app-launch/
	install -m 755 desktop-exec $(DESTDIR)/usr/lib/$(DEB_BUILD_MULTIARCH)/upstart-app-launch/
	install -m 755 zg-report-app $(DESTDIR)/usr/lib/$(DEB_BUILD_MULTIARCH)/upstart-app-launch/
	mkdir -p $(DESTDIR)/usr/bin/
	install -m 755 lsapp $(DESTDIR)/usr/bin/

check: application-legacy.conf
	@echo " *** Checking Application Job *** "
	@init-checkconf application.conf
	@echo " *** Checking Application Click Job *** "
	@init-checkconf application-click.conf
	@echo " *** Checking Application Legacy Job *** "
	@init-checkconf application-legacy.conf
