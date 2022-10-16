package com.rusefi.server;

import org.takes.Request;
import org.takes.Response;
import org.takes.Take;
import org.takes.facets.fork.FkRegex;
import org.takes.facets.fork.TkFork;
import org.takes.http.Exit;
import org.takes.http.Front;
import org.takes.http.FtBasic;
import org.takes.misc.Href;
import org.takes.rq.RqHref;
import org.takes.rs.RsJson;

import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonObject;

public class TakesServerSandbox {

    public static void main(final String... args) throws Exception {
        Take fork = new TkFork(
                new FkRegex("/list", new Take() {
                    @Override
                    public Response act(Request request) throws Exception {
                        Href href = new RqHref.Base(request).href();
//                                URI uri = href.uri();
                        Iterable<String> values = href.param("user");
                        String name = values.iterator().next();


                        JsonArray result = Json.createArrayBuilder()
                                .add(Json.createObjectBuilder().add("name", name))
                                .add(Json.createObjectBuilder().add("name", name))
                                .build();

                        return new RsJson(result);


                    }
                }),
                new FkRegex("/", "hello, world!")
        );
        Front ftBasic = new FtBasic(fork, 8080);
        ftBasic.start(Exit.NEVER);
    }


    public static final class User implements RsJson.Source {
        private final String name;

        public User(String name) {
            this.name = name;
        }

        @Override
        public JsonObject toJson() {
            return Json.createObjectBuilder()
                    .add("name", name)
                    .build();
        }
    }

}
