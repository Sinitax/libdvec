LIBDVEC_1.1 {
	global:
		dvec_init;
		dvec_deinit;

		dvec_alloc;
		dvec_free;

		dvec_copy;
		dvec_swap;

		dvec_clear;
		dvec_reserve;
		dvec_shrink;

		dvec_add;
		dvec_remove;
		dvec_replace;

		dvec_iter_fwd;
		dvec_iter_bwd;
	local: *;
};
