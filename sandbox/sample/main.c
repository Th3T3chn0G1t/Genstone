// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genlocale.h>

int main() {
	glog(INFO, "Hello, Genstone!");

	size_t size = 0;
	(void) gen_locale_convert_currency(NULL, &size, 3, 40);
	char amount[size];
	(void) gen_locale_convert_currency(amount, NULL, 3, 40);

	glogf(DEBUG, "3 units 40 cents -> %s", amount);
}
