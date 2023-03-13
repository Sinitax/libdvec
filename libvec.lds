LIBVEC_1.3 {
	global:
		vec_init;
		vec_deinit;

		vec_alloc;
		vec_free;

		vec_clear;
		vec_resize;
		vec_shrink;

		vec_reserve;
		vec_remove;
		vec_replace;

		vec_iter_fwd;
		vec_iter_bwd;
	local: *;
};
