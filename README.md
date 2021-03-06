# GREYHOUND

[![Build Status](https://api.travis-ci.org/hobu/greyhound.svg?branch=master)](https://travis-ci.org/hobu/greyhound)

A point cloud streaming framework for dynamic web services and native applications.

See it in action with the dynamic [Plas.io](http://speck.ly) client at [speck.ly](http://speck.ly) and the [Potree](http://potree.org) client at [potree.entwine.io](http://potree.entwine.io).

# Getting started

## Obtaining Greyhound

### Using Docker
```bash
docker run -it -p 8080:8080 connormanning/greyhound
```

### Natively

Prior to installing natively, you must first install [PDAL](https://pdal.io) and its dependencies, and then install [Entwine](https://entwine.io).  Then you can install Greyhound via NPM.

```bash
npm install -g greyhound-server
greyhound
```

## Indexing some data

Greyhound uses data indexed by [Entwine](https://entwine.io/).  See [the instructions](https://github.com/connormanning/entwine) for how to use Entwine.  If you are eager to get started, we have some publicly hosted data you can index and serve locally:

```bash
docker run -it -v $HOME:$HOME connormanning/entwine build \
    -i https://entwine.io/sample-data/red-rocks.laz \
    -o ~/greyhound/red-rocks
```

## Viewing the data

You've just indexed a LAZ file from the internet (data credit to [DroneMapper](https://dronemapper.com/sample_data)) and created a local Entwine dataset.  It's sitting at `~/greyhound/red-rocks`.  Now let's start Greyhound and take a look at the data.  We'll map our top-level output directory into one of the default search paths for the Greyhound container.

```bash
docker run -it -p 8080:8080 -v ~/greyhound:/greyhound connormanning/greyhound
```

Now that Greyhound is awake, you should be able to browse your data with [Plasio](http://speck.ly/?s=http://localhost:8080/&r=red-rocks) or [Potree](http://potree.entwine.io/data/custom.html?s=localhost:8080&r=red-rocks).

# Further reading
See the [client documentation](https://github.com/hobu/greyhound/blob/master/doc/source/clientDevelopment.rst) if you are interested in developing an application that streams data from Greyhound.  For instructions regarding configuring and deploying Greyhound, see the [administrator documentation](https://github.com/hobu/greyhound/blob/master/doc/source/administration.rst).

