echo "c/cpp files:"
cat `find . -name "*.c*"` | wc -l

echo "h files:"
cat `find . -name "*.h"` | wc -l

echo "txt files:"
cat `find . -name "*.txt"` | wc -l

