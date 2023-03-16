LIBDVEC_1.0 {
	global:
		dvec_init;
		dvec_deinit;

		dvec_alloc;
		dvec_free;

		dvec_clear;
		dvec_resize;
		dvec_shrink;

		dvec_reserve;
		dvec_remove;
		dvec_replace;

		dvec_iter_fwd;
		dvec_iter_bwd;
	local: *;
};
