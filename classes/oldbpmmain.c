
int main(int argc, char *argv[])
{
	float *nrg = NULL;
	size_t len = 0, buf = 0;
	off_t n = 0;
	double bpm, min = LOWER, max = UPPER, v = 0.0;
	const char *format = "%0.3f";
	FILE *fdiff = NULL, *fnrg = NULL;

	for (;;) {
		int c;

		c = getopt(argc, argv, "vf:g:e:m:x:h");
		if (c == -1)
			break;

		switch (c) {
		case 'f':
			format = optarg;
			break;

		case 'g':
			fdiff = fopen(optarg, "w");
			if (fdiff == NULL) {
				perror(optarg);
				return -1;
			}
			break;

		case 'e':
			fnrg = fopen(optarg, "w");
			if (fnrg == NULL) {
				perror(optarg);
				return -1;
			}
			break;

		case 'm':
			min = atof(optarg);
			break;

		case 'x':
			max = atof(optarg);
			break;

		case 'h':
			usage(stdout);
			return 0;

		default:
			return EX_USAGE;
		}
	}

	argv += optind;
	argc -= optind;

	if (argc > 0) {
		fprintf(stderr, "%s: Too many arguments\n", NAME);
		return EX_USAGE;
	}

	for (;;) {
		float z;

		if (fread(&z, sizeof z, 1, stdin) != 1)
			break;

		// Maintain an energy meter (similar to PPM) 

		z = fabsf(z);
		if (z > v) {
			v += (z - v) / 8;
		} else {
			v -= (v - z) / 512;
		}

		// At regular intervals, sample the energy to give a
		// low-resolution overview of the track 

		n++;
		if (n != INTERVAL)
			continue;

		n = 0;

		if (len == buf) {
			size_t n;

			n = buf + BLOCK;
			nrg = realloc(nrg, n * sizeof(*nrg));
			if (nrg == NULL) {
				perror("realloc");
				return -1;
			}
			buf = n;
		}

		if (fnrg != NULL) {
			fprintf(fnrg, "%lf\t%lf\n",
				(double)len * INTERVAL / RATE, v);
		}

		nrg[len++] = v;
	}

	bpm = scan_for_bpm(nrg, len, min, max, 1024, 1024, fdiff);

	printf(format, bpm);
	putc('\n', stdout);

	free(nrg);

	if (fdiff != NULL) {
		if (fclose(fdiff) != 0)
			perror("fclose");
	}

	if (fnrg != NULL) {
		if (fclose(fnrg) != 0)
			perror("fclose");
	}

	return 0;
}