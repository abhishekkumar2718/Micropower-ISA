FILES=examples/*
for f in $FILES
do
  printf "$f\n"
  ./bin/u_asm $f
  printf "\n"
done
