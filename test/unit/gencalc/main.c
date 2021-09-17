#include <gencalc.h>

int main() {
	glog(INFO, "Testing vector support...");
	const gfloat4 vec = {1.0f, 2.0f, 3.0f, 4.0f};
	GEN_DIAG_REGION_BEGIN
	GEN_DIAG_IGNORE_FP_EQ // We know that floating-point comparison is safe here
	GEN_REQUIRE_EQUAL((double) 1.0f, (double) vec.x);
	GEN_REQUIRE_EQUAL((double) 2.0f, (double) vec.y);
	GEN_REQUIRE_EQUAL((double) 3.0f, (double) vec.z);
	GEN_REQUIRE_EQUAL((double) 4.0f, (double) vec.w);
	GEN_DIAG_REGION_END

	glog(INFO, "Testing matrix support...");
	const gfloat4x4 mat;
	// Clang matrices don't like static initialization yet
	const float mat_data[] = {1.1f, 1.2f, 1.3f, 1.4f, 2.1f, 2.2f, 2.3f, 2.4f, 3.1f, 3.2f, 3.3f, 3.4f, 4.1f, 4.2f, 4.3f, 4.4f};
	GEN_FOREACH_PTR(i, f, sizeof(mat_data) / sizeof(float), mat_data) {
		mat[i / 4][i % 4] = *f;
	}

	GEN_DIAG_REGION_BEGIN
	GEN_DIAG_IGNORE_FP_EQ
	GEN_REQUIRE_EQUAL((double) 1.1f, (double) mat[0][0]);
	GEN_REQUIRE_EQUAL((double) 1.2f, (double) mat[0][1]);
	GEN_REQUIRE_EQUAL((double) 1.3f, (double) mat[0][2]);
	GEN_REQUIRE_EQUAL((double) 1.4f, (double) mat[0][3]);
	GEN_REQUIRE_EQUAL((double) 2.1f, (double) mat[1][0]);
	GEN_REQUIRE_EQUAL((double) 2.2f, (double) mat[1][1]);
	GEN_REQUIRE_EQUAL((double) 2.3f, (double) mat[1][2]);
	GEN_REQUIRE_EQUAL((double) 2.4f, (double) mat[1][3]);
	GEN_REQUIRE_EQUAL((double) 3.1f, (double) mat[2][0]);
	GEN_REQUIRE_EQUAL((double) 3.2f, (double) mat[2][1]);
	GEN_REQUIRE_EQUAL((double) 3.3f, (double) mat[2][2]);
	GEN_REQUIRE_EQUAL((double) 3.4f, (double) mat[2][3]);
	GEN_REQUIRE_EQUAL((double) 4.1f, (double) mat[3][0]);
	GEN_REQUIRE_EQUAL((double) 4.2f, (double) mat[3][1]);
	GEN_REQUIRE_EQUAL((double) 4.3f, (double) mat[3][2]);
	GEN_REQUIRE_EQUAL((double) 4.4f, (double) mat[3][3]);
	GEN_DIAG_REGION_END
}
