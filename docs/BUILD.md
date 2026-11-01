# Compilar e validar

Instale Visual Studio 2022 com Desktop development with C++, MSVC v143 x64/x86,
Windows SDK 10/11, NuGet CLI e Python 3. Abra `Unibox.sln` e selecione Release/x64.
Use o x64 Native Tools Command Prompt ou Developer PowerShell configurado para
x64. Na raiz do checkout, execute em ordem e interrompa ao primeiro erro:

```powershell
python scripts/validate_unibox.py
python -m unittest discover -s tests -p "test_*.py" -v
nuget restore Unibox.sln -ConfigFile NuGet.config -NonInteractive
python scripts/validate_unibox.py --require-packages
cl /nologo /EHsc /std:c++20 tests\radar_math_tests.cpp /Fe:calculation_tests.exe
.\calculation_tests.exe
cl /nologo /EHsc /utf-8 /std:c++20 /Ipackages\boost.1.87.0\lib\native\include tests\config_codec_tests.cpp /Fe:config_codec_tests.exe
.\config_codec_tests.exe
msbuild Unibox.sln /m:2 /p:Configuration=Release /p:Platform=x64 /bl:msbuild-Release.binlog
python scripts/verify_pe.py output/x64/Release/Uniboxx64Release.dll
python scripts/package_delivery.py --dll output/x64/Release/Uniboxx64Release.dll --output artifacts/Amalgam-Ultimate-Release.zip
```

Para AVX2, use `/p:Configuration=ReleaseAVX2`; saída:
`output/x64/ReleaseAVX2/Uniboxx64ReleaseAVX2.dll`. Preserve apenas essa variante
para CPUs que suportem AVX2. ReleaseTextmode e ReleaseTextmodeAVX2 continuam
configurações originais; não desenham o overlay ImGui.

A única dependência NuGet é Boost 1.87.0. ImGui e MinHook já estão nos fontes.
Não copie arquivos dos forks de referência nem instale CPR/libcurl/libolm.
O par ProfileStalker é opcional e não foi distribuído originalmente: as guardas
existentes permanecem, sem stub. Restaurar apenas um dos dois arquivos causa
falha explícita. A ausência do par não é uma falha de build obrigatória.

`.github/workflows/msbuild.yml` faz restore, preflight, testes Python/C++ MSVC,
build/link, verificação AMD64/PE32+/DLL e publica DLL/PDB/logs/pacotes. O pacote
inclui manifest com commit e SHA256 da DLL. O ZIP de fontes é `git archive` da
revisão exata, extraído e validado sem depender de arquivos fora do checkout.

No Linux, execute o preflight, unittest e:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -pthread tests/radar_math_tests.cpp -o /tmp/unibox-tests
/tmp/unibox-tests
# Boost 1.87.0 headers precisam estar no include path:
g++ -std=c++20 -Wall -Wextra -Werror -I/path/to/boost tests/config_codec_tests.cpp -o /tmp/config-tests
/tmp/config-tests
python3 scripts/package_delivery.py --source HEAD --output /tmp/Amalgam-Ultimate-source.zip
```

Esses comandos Linux não compilam a DLL Windows. A validação PE não comprova
carregamento no jogo. Consulte LOCAL-TESTING.md para registrar testes locais.
