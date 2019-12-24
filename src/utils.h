/**
 * Provides basic useful functions.
 */
#ifndef UTILS_H
#define UTILS_H

/**
 * Returns the maximum of two unsigned ints.
 *
 * @param x  The first number
 * @param y  The second number
 * @return   The maximum between `x` and `y`
 */
unsigned int max(unsigned int x, unsigned int y);

/**
 * Returns the minimum of two unsigned ints.
 *
 * @param x  The first number
 * @param y  The second number
 * @return   The minimum between `x` and `y`
 */
unsigned int min(unsigned int x, unsigned int y);

/**
 * Counts the number of occurrences of a character in a string.
 *
 * @param s  The string
 * @param c  The character
 * @return   The number of occurrences
 */
unsigned int num_occurrences(const char *s, char c);

/**
 * Modulo operator.
 *
 * @param a  The dividend
 * @param b  The divisor
 * @return   A positive remainder
 */
int mod(int a, int b);

/**
 * Duplicates a string.
 *
 * @param s  The string to duplicate
 * @return   A copy of the string
 */
char *strdupli(const char *s);

#endif
