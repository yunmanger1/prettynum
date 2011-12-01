#include "Python.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// dynamic programming
// length, mask, last, prelast
#define MAXLEN 7
#define MASK 59049
#define TEN 10
// 3^10

int dp[MAXLEN][MASK][TEN][TEN];
int three[TEN]; // 3^n
int u[MAXLEN][MASK][TEN][TEN];
int n = 7; // length of objects

int go(int len, int mask, int last, int prelast) {
	if (len == n)
		return 1;
	if (u[len][mask][last][prelast])
		return dp[len][mask][last][prelast];
	u[len][mask][last][prelast] = 1;

	int res = dp[len][mask][last][prelast];
	int cnt[10] = {0};

	int cmask = mask;
	int i = 0;
	for (; i < 10; i++) {
		cnt[i] = cmask % 3;
		cmask /= 3;
	}

	i = 0;
	for (; i < 10; i++)
		if (cnt[i] != 2 // if i was used less than 2 times
				&& (len < 1 || last != i) // previous wasn't i
				&& (len < 2 || prelast != i)) // and before it wasn't i
		{
			cmask = mask + three[i];
			res += go(len + 1, cmask, i, last);
		}
	dp[len][mask][last][prelast] = res;
	return res;
}

void fillthree(){
	if (three[0] != 1){
		three[0] = 1;
		int i = 1;
		for (; i < 10; i++)
			three[i] = three[i - 1] * 3;
	}
}

// finds kth object (numeration starts from 0)
int id2num(int k) {
	fillthree();
	assert(k < go(0,0,0,0));

	int res = 0;
	int last = 0, prelast = 0, mask = 0, cnt[10] = {0};
	int i = 0;
	for (; i < n; i++) {
		int j = 0;
		for (; j < 10; j++)
			if (cnt[j] < 2
				&& (i < 1 || last != j)
				&& (i < 2 || prelast != j)) {

				if (k >= go(i + 1, mask + three[j], j, last))
					k -= go(i + 1, mask + three[j], j, last);
				else {
					mask += three[j];
					cnt[j]++;
					prelast = last;
					last = j;
					res = res * 10 + j;
					break;
				}

			}
		assert(j < 10);
	}
	return res;
}

// returns number of object in lexig. order (starting from 0)
int num2id(int k) {
	fillthree();
	int res = 0, last = 0, prelast = 0, mask = 0, cnt[10] = {0};
	char s[20];
	sprintf(s, "%07d", k);
	assert(strlen(s) == n);
	int i = 0;
	for (; i < n; i++) {
		int num = s[i] - '0';
		int j = 0;
		for (; j < num; j++)
			if (cnt[j] < 2
				&& (i < 1 || last != j)
				&& (i < 2 || prelast != j))
				res += go(i + 1, mask + three[j], j, last);

		mask += three[num];
		cnt[num]++;
		prelast = last;
		last = num;
	}
	return res;
}

/* The module doc string */
PyDoc_STRVAR(prettynum__doc__,
"Getting pretty number by id and id by pretty number");

/* The function doc string */
PyDoc_STRVAR(id2num__doc__,
"get pretty number by id");

/* The function doc string */
PyDoc_STRVAR(num2id__doc__,
"get id by pretty number ");

/* The wrapper to the underlying C function */
static PyObject *
py_id2num(PyObject *self, PyObject *args)
{
	int result, id;
	/* "args" must have two doubles and may have an integer */
	/* If not specified, "max_iterations" remains unchanged; defaults to 1000 */
	/* The ':iterate_point' is for error messages */
	if (!PyArg_ParseTuple(args, "i", &id))
		return NULL;
	/* Call the C function */
	result = id2num(id);

	/* Convert from a C integer value to a Python integer instance */
	return PyInt_FromLong((long) result);
}

/* The wrapper to the underlying C function */
static PyObject *
py_num2id(PyObject *self, PyObject *args)
{
	int result, num=0;
	/* "args" must have two doubles and may have an integer */
	/* If not specified, "max_iterations" remains unchanged; defaults to 1000 */
	/* The ':iterate_point' is for error messages */
	if (!PyArg_ParseTuple(args, "i", &num))
		return NULL;
	/* Call the C function */
	result = num2id(num);

	/* Convert from a C integer value to a Python integer instance */
	return PyInt_FromLong((long) result);
}

/* A list of all the methods defined by this module. */
/* "iterate_point" is the name seen inside of Python */
/* "py_iterate_point" is the name of the C function handling the Python call */
/* "METH_VARGS" tells Python how to call the handler */
/* The {NULL, NULL} entry indicates the end of the method definitions */
static PyMethodDef prettynum_methods[] = {
	{"id2num",  py_id2num, METH_VARARGS, id2num__doc__},
	{"num2id",  py_num2id, METH_VARARGS, num2id__doc__},
	{NULL, NULL}      /* sentinel */
};

/* When Python imports a C module named 'X' it loads the module */
/* then looks for a method named "init"+X and calls it.  Hence */
/* for the module "mandelbrot" the initialization function is */
/* "initmandelbrot".  The PyMODINIT_FUNC helps with portability */
/* across operating systems and between C and C++ compilers */
PyMODINIT_FUNC
initprettynum(void)
{
	/* There have been several InitModule functions over time */
	Py_InitModule3("prettynum", prettynum_methods,
			prettynum__doc__);
}
