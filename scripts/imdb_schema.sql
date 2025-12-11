-- CREATE DATABASE imdb;
--
-- taken from https://developer.imdb.com/non-commercial-datasets/
--

-- create enum for titleType: ATTENTION: New values may be added in the future without warning
CREATE TYPE title_basics_title_type AS ENUM (
	'movie',
	'short',
	'tvEpisode',
	'tvMiniSeries',
	'tvMovie',
	'tvSeries',
	'tvShort',
	'tvPilot',
	'tvSpecial',
	'video',
	'videoGame'
);

-- create enum for genres: ATTENTION: New values may be added in the future without warning
CREATE TYPE title_basics_genres AS ENUM (
	'Game-Show',
	'Family',
	'Music',
	'Reality-TV',
	'Comedy',
	'Western',
	'Short',
	'Crime',
	'War',
	'Romance',
	'Biography',
	'Drama',
	'Mystery',
	'Sci-Fi',
	'Fantasy',
	'Adventure',
	'Documentary',
	'Action',
	'Animation',
	'Sport',
	'Horror',
	'Adult',
	'News',
	'Talk-Show',
	'Film-Noir',
	'Musical',
	'Thriller',
	'History'
);

-- create the table title_basics
CREATE TABLE IF NOT EXISTS public.title_basics (
	tconst TEXT NOT NULL,
	titleType title_basics_title_type NOT NULL,
	primaryTitle TEXT NOT NULL,
	originalTitle TEXT NOT NULL,
	isAdult boolean NOT NULL,
	startYear INT,
	endYear INT,
	runTimeMinutes INT,
	genres title_basics_genres [],
	PRIMARY KEY(tconst)
);

-- create enum for category: ATTENTION: New values may be added in the future without warning
CREATE TYPE general_job AS ENUM (
	'director',
	'cinematographer',
	'composer',
	'editor',
	'actor',
	'miscellaneous',
	'producer',
	'actress',
	'writer',
	'soundtrack',
	'archive_footage',
	'music_department',
	'stunts',
	'make_up_department',
	'assistant_director',
	'casting_department',
	'music_artist',
	'camera_department',
	'art_department',
	'casting_director',
	'executive',
	'costume_designer',
	'script_department',
	'art_director',
	'editorial_department',
	'costume_department',
	'animation_department',
	'talent_agent',
	'archive_sound',
	'special_effects',
	'production_manager',
	'production_designer',
	'sound_department',
	'visual_effects',
	'location_management',
	'set_decorator',
	'transportation_department',
	'choreographer',
	'manager',
	'publicist',
	'podcaster',
	'legal',
	'assistant',
	'production_department',
	'electrical_department',
	'accountant',
	'self'
	-- TODO: add the rest
);

-- create the table name_basics
CREATE TABLE IF NOT EXISTS public.name_basics (
	nconst TEXT NOT NULL,
	primaryName TEXT NOT NULL,
	birthYear INT,
	deathYear INT,
	primaryProfession general_job [] NOT NULL,
	knownForTitles TEXT [] NOT NULL,
	PRIMARY KEY (nconst)
	-- FOREIGN KEY (EACH ELEMENT OF knownForTitles) REFERENCES public.title_basics(tconst)

	-- EACH ELEMENT not yet supported
	-- see: https://stackoverflow.com/questions/41054507/postgresql-array-of-elements-that-each-are-a-foreign-key
);

-- create enum for type: ATTENTION: New values may be added in the future without warning
CREATE TYPE title_akas_type AS ENUM (
	'alternative',
	'dvd',
	'festival',
	'tv',
	'video',
	'working',
	'original',
	'imdbDisplay'
);

-- create the table title_akas
CREATE TABLE IF NOT EXISTS public.title_akas (
	titleId TEXT NOT NULL,
	ordering INT NOT NULL,
	title TEXT NOT NULL,
	region TEXT,
	language TEXT,
	types title_akas_type [] NOT NULL,
	attributes TEXT [] NOT NULL,
	isOriginalTitle BOOLEAN NOT NULL,
	PRIMARY KEY(titleId, ordering),
	FOREIGN KEY (titleId) REFERENCES public.title_basics(tconst)
);

-- create the table title_crew
CREATE TABLE IF NOT EXISTS public.title_crew (
	tconst TEXT NOT NULL,
	directors TEXT [] NOT NULL,
	writers TEXT [] NOT NULL,
	PRIMARY KEY(tconst),
	-- FOREIGN KEY (EACH ELEMENT OF directors) REFERENCES public.name_basics(nconst),
	-- FOREIGN KEY (EACH ELEMENT OF writers) REFERENCES public.name_basics(nconst),
	FOREIGN KEY (tconst) REFERENCES public.title_basics(tconst)
);

-- create the table title_episode
CREATE TABLE IF NOT EXISTS public.title_episode (
	tconst TEXT NOT NULL,
	parentTconst TEXT NOT NULL,
	seasonNumber INT,
	episodeNumber INT,
	PRIMARY KEY(tconst),
	FOREIGN KEY (tconst) REFERENCES public.title_basics(tconst),
	FOREIGN KEY (parentTconst) REFERENCES public.title_basics(tconst)
);

-- create the table title_principals
CREATE TABLE IF NOT EXISTS public.title_principals (
	tconst TEXT NOT NULL,
	ordering INT NOT NULL,
	nconst TEXT NOT NULL,
	category general_job NOT NULL,
	job TEXT,
	characters TEXT,
	PRIMARY KEY(tconst, ordering),
	FOREIGN KEY (nconst) REFERENCES public.name_basics(nconst),
	FOREIGN KEY (tconst) REFERENCES public.title_basics(tconst)
);

-- create the table title_ratings
CREATE TABLE IF NOT EXISTS public.title_ratings (
	tconst TEXT NOT NULL,
	averageRating NUMERIC NOT NULL,
	numVotes INT NOT NULL,
	PRIMARY KEY(tconst),
	FOREIGN KEY (tconst) REFERENCES public.title_basics(tconst)
);

-- create the table imdb_importer_metadata
CREATE TABLE IF NOT EXISTS public.imdb_importer_metadata (
	id SERIAL NOT NULL,
	version INT NOT NULL,
	versionString TEXT NOT NULL,
	description TEXT NOT NULL,
	timestamp TIMESTAMP NOT NULL,
	options TEXT,
	PRIMARY KEY(id)
);


