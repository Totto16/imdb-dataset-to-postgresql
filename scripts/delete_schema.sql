
-- DELETE DATABASE imdb with all custom types

--delete tables, in reverse order
DROP TABLE IF EXISTS public.title_ratings;
DROP TABLE IF EXISTS public.title_principals;
DROP TABLE IF EXISTS public.title_episode;
DROP TABLE IF EXISTS public.title_crew;
DROP TABLE IF EXISTS public.title_akas;
DROP TABLE IF EXISTS public.name_basics;
DROP TABLE IF EXISTS public.title_basics;

DROP TABLE IF EXISTS public.imdb_importer_metadata;

-- delete enum types
DROP TYPE IF EXISTS title_basics_title_type;
DROP TYPE IF EXISTS title_basics_genres;
DROP TYPE IF EXISTS general_job;
DROP TYPE IF EXISTS title_akas_type;
