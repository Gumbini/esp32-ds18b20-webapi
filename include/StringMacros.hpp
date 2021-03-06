#pragma once
#ifndef STRINGMACROS_HPP
#define STRINGMACROS_HPP

#define STRING_EQUALS(X, Y)          (!strcmp(X, Y))
#define STRING_MATCH_FIRST(X, Y, N)  (!strncmp(X, Y, N))
#define STRING_STARTS_WITH(X, Y)     (!strncmp(X, Y, strlen(Y)))
#define STRING_PRUNE_SUBSTRING(X, Y) (X += strlen(Y))

#endif // STRINGMACROS_HPP
