_SPIRV_COMMON_CFLAGS = -Igenstone/vendor/SPIRV-Headers/include -Igenstone/vendor/SPIRV-Reflect -Igenstone/vendor/SPIRV-Tools/include -Igenstone/vendor/SPIRV-Headers/include/spirv/unified1/

_SPIRV_CFLAGS = $(_SPIRV_COMMON_CFLAGS) -Igenstone/vendor/SPIRV-Tools -Igenstone/vendor
_SPIRV_CXXFLAGS = $(_SPIRV_COMMON_CFLAGS) $(_SPIRV_COMMON_CXXFLAGS)

SPIRV_CFLAGS = $(_SPIRV_COMMON_CFLAGS)
SPIRV_LFLAGS = -lspirv

SPIRV_INSTS = genstone/vendor/SPIRV-Tools/source/debuginfo.insts.inc genstone/vendor/SPIRV-Tools/source/nonsemantic.clspvreflection.insts.inc genstone/vendor/SPIRV-Tools/source/nonsemantic.shader.debuginfo.100.insts.inc genstone/vendor/SPIRV-Tools/source/opencl.debuginfo.100.insts.inc genstone/vendor/SPIRV-Tools/source/spv-amd-gcn-shader.insts.inc genstone/vendor/SPIRV-Tools/source/spv-amd-shader-ballot.insts.inc genstone/vendor/SPIRV-Tools/source/spv-amd-shader-explicit-vertex-parameter.insts.inc  genstone/vendor/SPIRV-Tools/source/spv-amd-shader-trinary-minmax.insts.inc
SPIRV_GENERATED = genstone/vendor/SPIRV-Tools/source/enum_string_mapping.inc genstone/vendor/SPIRV-Tools/source/extension_enum.inc genstone/vendor/SPIRV-Tools/source/opencl.std.insts.inc genstone/vendor/SPIRV-Tools/source/glsl.std.450.insts.inc genstone/vendor/SPIRV-Tools/source/core.insts-unified1.inc genstone/vendor/SPIRV-Tools/source/operand.kinds-unified1.inc genstone/vendor/SPIRV-Tools/source/generators.inc
SPIRV_C_SOURCES = $(wildcard genstone/vendor/SPIRV-Reflect/*.c)
SPIRV_CXX_SOURCES = $(wildcard genstone/vendor/SPIRV-Tools/source/*.cpp) $(wildcard genstone/vendor/SPIRV-Tools/source/val/*.cpp) $(wildcard genstone/vendor/SPIRV-Tools/source/util/*.cpp)
SPIRV_OBJECTS = $(SPIRV_C_SOURCES:.c=$(OBJECT_SUFFIX)) $(SPIRV_CXX_SOURCES:.cpp=$(OBJECT_SUFFIX))

SPIRV_LIB = lib/$(LIB_PREFIX)spirv$(DYNAMIC_LIB_SUFFIX)

build_message_spirv:
	@$(ECHO) "$(SECTION_PREFIX) SPIRV"
	@$(ECHO) "$(INFO_PREFIX) Khronos helps us look at shaders"

spirv: build_message_spirv $(SPIRV_LIB) ### @Vendor Builds SPIRV as a Genstone module

$(SPIRV_LIB): CXXFLAGS = $(_SPIRV_CFLAGS)
$(SPIRV_LIB): CFLAGS = $(_SPIRV_CFLAGS)
$(SPIRV_LIB): LFLAGS = -Llib
$(SPIRV_LIB): CLANG_FORMAT = DISABLED
$(SPIRV_LIB): CFISAN_FLAGS = -fno-lto
$(SPIRV_LIB): $(SPIRV_OBJECTS) | lib

$(SPIRV_CXX_SOURCES): $(SPIRV_GENERATED) $(SPIRV_INSTS)

$(SPIRV_GENERATED):
	@$(ECHO) "$(ACTION_PREFIX)"
	$(PYTHON3) genstone/vendor/SPIRV-Tools/utils/generate_grammar_tables.py --spirv-core-grammar=genstone/vendor/SPIRV-Headers/include/spirv/unified1/spirv.core.grammar.json --extension-enum-output=genstone/vendor/SPIRV-Tools/source/extension_enum.inc --enum-string-mapping-output=genstone/vendor/SPIRV-Tools/source/enum_string_mapping.inc --extinst-debuginfo-grammar=genstone/vendor/SPIRV-Headers/include/spirv/unified1/extinst.debuginfo.grammar.json --extinst-cldebuginfo100-grammar=genstone/vendor/SPIRV-Headers/include/spirv/unified1/extinst.opencl.debuginfo.100.grammar.json
	$(PYTHON3) genstone/vendor/SPIRV-Tools/utils/generate_grammar_tables.py --spirv-core-grammar=genstone/vendor/SPIRV-Headers/include/spirv/unified1/spirv.core.grammar.json --extinst-debuginfo-grammar=genstone/vendor/SPIRV-Headers/include/spirv/unified1/extinst.debuginfo.grammar.json --extinst-cldebuginfo100-grammar=genstone/vendor/SPIRV-Headers/include/spirv/unified1/extinst.opencl.debuginfo.100.grammar.json --core-insts-output=genstone/vendor/SPIRV-Tools/source/core.insts-unified1.inc --operand-kinds-output=genstone/vendor/SPIRV-Tools/source/operand.kinds-unified1.inc
	$(PYTHON3) genstone/vendor/SPIRV-Tools/utils/generate_grammar_tables.py --extinst-opencl-grammar=genstone/vendor/SPIRV-Headers/include/spirv/unified1/extinst.opencl.std.100.grammar.json --opencl-insts-output=genstone/vendor/SPIRV-Tools/source/opencl.std.insts.inc
	$(PYTHON3) genstone/vendor/SPIRV-Tools/utils/generate_grammar_tables.py --extinst-glsl-grammar=genstone/vendor/SPIRV-Headers/include/spirv/unified1/extinst.glsl.std.450.grammar.json --glsl-insts-output=genstone/vendor/SPIRV-Tools/source/glsl.std.450.insts.inc
	$(PYTHON3) genstone/vendor/SPIRV-Tools/utils/generate_registry_tables.py --xml=genstone/vendor/SPIRV-Headers/include/spirv/spir-v.xml --generator-output=genstone/vendor/SPIRV-Tools/source/generators.inc
	@$(ECHO) "$(ACTION_SUFFIX)"

genstone/vendor/SPIRV-Tools/source/opencl.debuginfo.100.insts.inc: _SPIRV_INST_OPERAND_TYPE = CLDEBUG100_
genstone/vendor/SPIRV-Tools/source/nonsemantic.shader.debuginfo.100.insts.inc: _SPIRV_INST_OPERAND_TYPE = SHDEBUG100_

%.insts.inc:
	@$(ECHO) "$(ACTION_PREFIX)"
	$(PYTHON3) genstone/vendor/SPIRV-Tools/utils/generate_grammar_tables.py --extinst-vendor-grammar=genstone/vendor/SPIRV-Headers/include/spirv/unified1/extinst.$(notdir $*).grammar.json --vendor-insts-output=$@ --vendor-operand-kind-prefix=$(_SPIRV_INST_OPERAND_TYPE)
	@$(ECHO) "$(ACTION_SUFFIX)"

clean_spirv:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(SPIRV_OBJECTS)
	-rm $(SPIRV_LIB)
	-rm $(SPIRV_INSTS)
	-rm $(SPIRV_GENERATED)
	@$(ECHO) "$(ACTION_SUFFIX)"
