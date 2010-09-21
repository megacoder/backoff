/*
 * vim: ts=8 sw=8
 */

#include <stdio.h>
#include <limits.h>

#define	min(x,y)	( ((x) < (y)) ? (x) : (y) )

unsigned	mult = 3;
unsigned	div = 2;
unsigned	offset = 5;

unsigned
backoff(
	unsigned delay,
	unsigned upto
)
{
	unsigned new;

	new = min( upto, delay + ( (delay*mult) / div ) + offset );
	return( new );
}

int
main(
	int		argc,
	char * *	argv
)
{
	static unsigned const	x_lim = (15 * 60);
	static unsigned const	x_incr = 5;
	static char const	dataFn[] = { "data" };
	unsigned		x;
	FILE *			script;
	FILE *			data;

	data = fopen( dataFn, "w" );

	for( x = 0; x < x_lim; x += x_incr )	{
		unsigned 	delay;
		unsigned 	lastDelay;
		unsigned 	generation;
		char		detailFn[ BUFSIZ ];
		FILE *		detail;

		snprintf(
			detailFn,
			sizeof( detailFn ),
			"details-%03u",
			x
		);

		detail = fopen( detailFn, "w" );
		delay = x;
		lastDelay = delay + 1;
		for( generation = 1; delay != lastDelay; ++generation )	{
			unsigned const	bo = backoff( delay, x_lim );

			fprintf(
				detail,
				"%u\t%u\n",
				generation,
				delay
			);
			lastDelay = delay;
			delay = bo;
		}
		fclose( detail );
		fprintf( data, "%u\t%u\n", x, generation );
	}
	fclose( data );

	script = fopen( "plot", "w" );
	fprintf( script, "set xlabel 'SEED (in seconds)'\n" );
	fprintf( script, "set ylabel 'GENERATIONS'\n" );
	fprintf( 
		script, 
		"plot \"%s\"",
		dataFn 
	);
	fprintf(
		script,
		" title 'delay + ((delay*%u)/%u) + %u'",
		mult,
		div,
		offset
	);
	fprintf( script, " with lines\n" );
	fprintf( script, "pause mouse\n" );
	fclose( script );
	return( 0 );
}
