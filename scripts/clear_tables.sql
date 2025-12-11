
-- CLEAR(TRUNCATE) DATABASE imdb

--delete tables, in correct order
TRUNCATE TABLE public.title_ratings CASCADE;
TRUNCATE TABLE public.title_principals CASCADE;
TRUNCATE TABLE public.title_episode CASCADE;
TRUNCATE TABLE public.title_crew CASCADE;
TRUNCATE TABLE public.title_akas CASCADE;
TRUNCATE TABLE public.name_basics CASCADE ;
TRUNCATE TABLE public.title_basics CASCADE;

TRUNCATE TABLE public.imdb_importer_metadata CASCADE;
