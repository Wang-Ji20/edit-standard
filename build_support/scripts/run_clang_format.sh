

cd ./lib ; find . -name '*.cc' -o -name '*.hh' | xargs clang-format -i

cd ../include ; find . -name '*.cc' -o -name '*.hh' | xargs clang-format -i
